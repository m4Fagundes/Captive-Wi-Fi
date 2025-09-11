// #include "infrastructure/persistence/NvsUserRepository.h"
// #include "esp_log.h" // Para logging (debug)
// #include "nvs_flash.h"
// #include "cJSON.h"
// #include <chrono>

// namespace infrastructure::persistence {

// static const char* TAG = "NvsUserRepository"; // Tag para os logs

// NvsUserRepository::NvsUserRepository() {
//     // Abre o namespace "users" no NVS. Todos os usuários serão salvos aqui.
//     esp_err_t err = nvs_open(nvs_namespace.c_str(), NVS_READWRITE, &nvs_handle);
//     if (err != ESP_OK) {
//         ESP_LOGE(TAG, "Erro (%s) ao abrir o handle do NVS!", esp_err_to_name(err));
//     } else {
//         ESP_LOGI(TAG, "Handle do NVS aberto com sucesso.");
//     }
// }

// NvsUserRepository::~NvsUserRepository() {
//     nvs_close(nvs_handle);
// }

// // Implementação do método save
// void NvsUserRepository::save(const domain::entities::User& user) {
//     // 1. Cria um objeto JSON
//     cJSON *root = cJSON_CreateObject();
    
//     // 2. Adiciona os campos do User ao JSON
//     cJSON_AddStringToObject(root, "id", user.getId().c_str());
//     cJSON_AddStringToObject(root, "name", user.getName().c_str());
//     cJSON_AddStringToObject(root, "email", user.getEmail().c_str());
//     cJSON_AddStringToObject(root, "provider", user.getProvider().c_str());
//     cJSON_AddStringToObject(root, "providerId", user.getProviderId().c_str());
    
//     long long lastLoginMs = std::chrono::duration_cast<std::chrono::milliseconds>(
//         user.getLastLogin().time_since_epoch()
//     ).count();
//     cJSON_AddNumberToObject(root, "lastLogin", lastLoginMs);

//     // 3. Converte o JSON para uma string
//     char *json_string = cJSON_PrintUnformatted(root);

//     // 4. Salva a string JSON no NVS usando o ID do usuário como chave
//     ESP_LOGI(TAG, "Salvando usuário ID: %s", user.getId().c_str());
//     esp_err_t err = nvs_set_str(nvs_handle, user.getId().c_str(), json_string);
//     if (err != ESP_OK) {
//         ESP_LOGE(TAG, "Erro (%s) ao salvar no NVS!", esp_err_to_name(err));
//     }

//     // 5. Libera a memória do JSON
//     free(json_string);
//     cJSON_Delete(root);

//     // 6. Commita as alterações para a flash
//     nvs_commit(nvs_handle);
// }

// // Método para encontrar usuários (exemplo simplificado)
// std::optional<domain::entities::User> NvsUserRepository::findByProviderId(const std::string& provider, const std::string& providerId) {
//     ESP_LOGW(TAG, "Função findByProviderId não está implementada de forma otimizada!");
//     // NOTA: O NVS é um simples mapa chave-valor. Fazer buscas por campos que não
//     // são a chave é ineficiente. A abordagem correta seria criar um índice separado.
//     // Para este exemplo, vamos manter simples e não implementar a busca por enquanto.
//     return std::nullopt;
// }

// std::optional<domain::entities::User> NvsUserRepository::findByEmail(const std::string& email) {
//      ESP_LOGW(TAG, "Função findByEmail não está implementada de forma otimizada!");
//     return std::nullopt;
// }

// } // namespace infrastructure::persistence