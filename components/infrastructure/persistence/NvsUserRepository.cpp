#include "infrastructure/persistence/NvsUserRepository.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "cJSON.h"
#include <chrono>
#include <vector>
#include <sstream>

namespace infrastructure::persistence {

static const char* TAG = "NvsUserRepository";

// Instância estática do Singleton
NvsUserRepository* NvsUserRepository::instance = nullptr;

// Construtor privado (padrão Singleton)
NvsUserRepository::NvsUserRepository() : is_initialized(false) {
    initialize();
}

// Destrutor: fecha os handles do NVS se inicializado
NvsUserRepository::~NvsUserRepository() {
    if (is_initialized) {
        nvs_close(nvs_handle);
        nvs_close(index_handle);
    }
}

// Método Singleton para obter instância
NvsUserRepository* NvsUserRepository::getInstance() {
    if (instance == nullptr) {
        instance = new NvsUserRepository();
    }
    return instance;
}

// Inicialização do repositório e namespaces no NVS
void NvsUserRepository::initialize() {
    // Inicializa partição principal do NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    
    // Abre namespace para dados dos usuários
    err = nvs_open(nvs_namespace.c_str(), NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro (%s) ao abrir namespace de usuários!", esp_err_to_name(err));
        return;
    }
    
    // Abre namespace para índices
    err = nvs_open(index_namespace.c_str(), NVS_READWRITE, &index_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro (%s) ao abrir namespace de índices!", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return;
    }
    
    is_initialized = true;
    ESP_LOGI(TAG, "Repositório NVS inicializado com sucesso");
}

// Factory Method: cria objeto User a partir de JSON
domain::entities::User NvsUserRepository::userFromJson(const std::string& json_string) const {
    cJSON* root = cJSON_Parse(json_string.c_str());
    if (!root) {
        throw std::runtime_error("Falha ao parsear JSON do usuário");
    }
    
    // Extrai campos obrigatórios
    cJSON* id_item = cJSON_GetObjectItem(root, "id");
    cJSON* name_item = cJSON_GetObjectItem(root, "name");
    cJSON* email_item = cJSON_GetObjectItem(root, "email");
    cJSON* provider_item = cJSON_GetObjectItem(root, "provider");
    cJSON* provider_id_item = cJSON_GetObjectItem(root, "providerId");
    
    if (!id_item || !name_item || !email_item || !provider_item || !provider_id_item) {
        cJSON_Delete(root);
        throw std::runtime_error("Campos obrigatórios ausentes no JSON do usuário");
    }
    
    std::string id = id_item->valuestring;
    std::string name = name_item->valuestring;
    std::string email = email_item->valuestring;
    std::string provider = provider_item->valuestring;
    std::string provider_id = provider_id_item->valuestring;
    
    cJSON_Delete(root);
    
    auto user = domain::entities::User::create(id, name, email, provider, provider_id);
    
    // Atualiza último login se presente
    cJSON* last_login = cJSON_GetObjectItem(root, "lastLogin");
    if (last_login && cJSON_IsNumber(last_login)) {
        // OBS: seria necessário adicionar setter em User para restaurar lastLogin
        // Aqui apenas atualiza para o tempo atual
        user.updateLastLogin();
    }
    
    return user;
}

// Converte User para string JSON
std::string NvsUserRepository::userToJson(const domain::entities::User& user) const {
    cJSON* root = cJSON_CreateObject();
    
    cJSON_AddStringToObject(root, "id", user.getId().c_str());
    cJSON_AddStringToObject(root, "name", user.getName().c_str());
    cJSON_AddStringToObject(root, "email", user.getEmail().c_str());
    cJSON_AddStringToObject(root, "provider", user.getProvider().c_str());
    cJSON_AddStringToObject(root, "providerId", user.getProviderId().c_str());
    
    long long lastLoginMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        user.getLastLogin().time_since_epoch()
    ).count();
    cJSON_AddNumberToObject(root, "lastLogin", lastLoginMs);
    
    char* json_string = cJSON_PrintUnformatted(root);
    std::string result(json_string);
    
    free(json_string);
    cJSON_Delete(root);
    
    return result;
}

// Cria chave de índice para provider + providerId
std::string NvsUserRepository::createProviderIndex(const std::string& provider, 
                                                   const std::string& providerId) const {
    return "p_" + provider + "_" + providerId;
}

// Cria chave de índice para email
std::string NvsUserRepository::createEmailIndex(const std::string& email) const {
    return "e_" + email;
}

// ------------------ Implementação das operações CRUD ------------------

// Salva usuário no NVS e cria índices
void NvsUserRepository::save(const domain::entities::User& user) {
    if (!is_initialized) {
        ESP_LOGE(TAG, "Repositório não inicializado!");
        return;
    }
    
    ESP_LOGI(TAG, "Salvando usuário: %s", user.getId().c_str());
    
    // Converte usuário para JSON
    std::string json_data = userToJson(user);
    
    // Salva dados do usuário
    esp_err_t err = nvs_set_str(nvs_handle, user.getId().c_str(), json_data.c_str());
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro ao salvar usuário: %s", esp_err_to_name(err));
        return;
    }
    
    // Cria índice para providerId
    std::string provider_key = createProviderIndex(user.getProvider(), user.getProviderId());
    err = nvs_set_str(index_handle, provider_key.c_str(), user.getId().c_str());
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Erro ao criar índice provider: %s", esp_err_to_name(err));
    }
    
    // Cria índice para email
    std::string email_key = createEmailIndex(user.getEmail());
    err = nvs_set_str(index_handle, email_key.c_str(), user.getId().c_str());
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Erro ao criar índice email: %s", esp_err_to_name(err));
    }
    
    // Commit das alterações
    nvs_commit(nvs_handle);
    nvs_commit(index_handle);
    
    ESP_LOGI(TAG, "Usuário salvo com sucesso");
}

// Busca usuário por providerId
std::optional<domain::entities::User> NvsUserRepository::findByProviderId(
    const std::string& provider, const std::string& providerId) {
    
    if (!is_initialized) {
        ESP_LOGE(TAG, "Repositório não inicializado!");
        return std::nullopt;
    }
    
    // Consulta no índice
    std::string index_key = createProviderIndex(provider, providerId);
    size_t length = 0;
    
    // Obtém tamanho do valor
    esp_err_t err = nvs_get_str(index_handle, index_key.c_str(), nullptr, &length);
    if (err != ESP_OK) {
        ESP_LOGD(TAG, "Usuário não encontrado por providerId");
        return std::nullopt;
    }
    
    // Lê ID do usuário
    std::vector<char> user_id_buffer(length);
    err = nvs_get_str(index_handle, index_key.c_str(), user_id_buffer.data(), &length);
    if (err != ESP_OK) {
        return std::nullopt;
    }
    
    std::string user_id(user_id_buffer.data());
    
    // Busca os dados completos do usuário
    return findById(user_id);
}

// Busca usuário por email
std::optional<domain::entities::User> NvsUserRepository::findByEmail(const std::string& email) {
    if (!is_initialized) {
        ESP_LOGE(TAG, "Repositório não inicializado!");
        return std::nullopt;
    }
    
    std::string index_key = createEmailIndex(email);
    size_t length = 0;
    
    esp_err_t err = nvs_get_str(index_handle, index_key.c_str(), nullptr, &length);
    if (err != ESP_OK) {
        ESP_LOGD(TAG, "Usuário não encontrado por email");
        return std::nullopt;
    }
    
    std::vector<char> user_id_buffer(length);
    err = nvs_get_str(index_handle, index_key.c_str(), user_id_buffer.data(), &length);
    if (err != ESP_OK) {
        return std::nullopt;
    }
    
    std::string user_id(user_id_buffer.data());
    return findById(user_id);
}

// Busca usuário por ID
std::optional<domain::entities::User> NvsUserRepository::findById(const std::string& id) {
    if (!is_initialized) {
        ESP_LOGE(TAG, "Repositório não inicializado!");
        return std::nullopt;
    }
    
    size_t length = 0;
    
    // Obtém tamanho do JSON
    esp_err_t err = nvs_get_str(nvs_handle, id.c_str(), nullptr, &length);
    if (err != ESP_OK) {
        ESP_LOGD(TAG, "Usuário não encontrado por ID: %s", id.c_str());
        return std::nullopt;
    }
    
    // Lê JSON do usuário
    std::vector<char> json_buffer(length);
    err = nvs_get_str(nvs_handle, id.c_str(), json_buffer.data(), &length);
    if (err != ESP_OK) {
        return std::nullopt;
    }
    
    std::string json_data(json_buffer.data());
    
    try {
        return userFromJson(json_data);
    } catch (const std::exception& e) {
        ESP_LOGE(TAG, "Erro ao desserializar usuário: %s", e.what());
        return std::nullopt;
    }
}

// Deleta usuário e remove índices
bool NvsUserRepository::deleteUser(const std::string& id) {
    if (!is_initialized) {
        ESP_LOGE(TAG, "Repositório não inicializado!");
        return false;
    }
    
    // Busca usuário antes de apagar para limpar índices
    auto user_opt = findById(id);
    if (!user_opt) {
        ESP_LOGW(TAG, "Usuário não encontrado para exclusão: %s", id.c_str());
        return false;
    }
    
    auto& user = *user_opt;
    
    // Remove índices
    std::string provider_key = createProviderIndex(user.getProvider(), user.getProviderId());
    nvs_erase_key(index_handle, provider_key.c_str());
    
    std::string email_key = createEmailIndex(user.getEmail());
    nvs_erase_key(index_handle, email_key.c_str());
    
    // Remove dados do usuário
    esp_err_t err = nvs_erase_key(nvs_handle, id.c_str());
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro ao deletar usuário: %s", esp_err_to_name(err));
        return false;
    }
    
    // Commit das alterações
    nvs_commit(nvs_handle);
    nvs_commit(index_handle);
    
    ESP_LOGI(TAG, "Usuário deletado com sucesso: %s", id.c_str());
    return true;
}

// Busca todos os usuários salvos
std::vector<domain::entities::User> NvsUserRepository::findAll() {
    std::vector<domain::entities::User> users;
    
    if (!is_initialized) {
        ESP_LOGE(TAG, "Repositório não inicializado!");
        return users;
    }
    
    // Itera sobre todas as entradas do namespace "users"
    nvs_iterator_t it = nullptr;
    esp_err_t err = nvs_entry_find(NVS_DEFAULT_PART_NAME, nvs_namespace.c_str(), 
                                   NVS_TYPE_STR, &it);
    
    while (err == ESP_OK) {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        
        auto user_opt = findById(std::string(info.key));
        if (user_opt) {
            users.push_back(*user_opt);
        }
        
        err = nvs_entry_next(&it);
    }
    
    nvs_release_iterator(it);
    
    ESP_LOGI(TAG, "Total de usuários encontrados: %d", users.size());
    return users;
}

} // namespace infrastructure::persistence
