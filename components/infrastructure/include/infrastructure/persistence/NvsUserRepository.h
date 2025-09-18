#pragma once

#include "domain/repositories/IUserRepository.h"
#include "nvs.h"
#include <string>
#include <vector>

namespace infrastructure::persistence {

/**
 * @brief Implementação concreta de IUserRepository utilizando o NVS (Non-Volatile Storage) do ESP32.
 * 
 * Esta classe aplica vários padrões de projeto:
 * 1. Repository Pattern - Abstrai a lógica de acesso a dados
 * 2. Singleton Pattern - Garante que exista apenas uma instância do repositório NVS
 * 3. Factory Method Pattern - Para criar objetos User a partir de JSON
 * 
 * A implementação utiliza dois namespaces do NVS:
 * - "users": Armazena os dados reais dos usuários em formato JSON
 * - "user_idx": Armazena índices para buscas eficientes por e-mail e ID de provedor
 */
class NvsUserRepository : public domain::repositories::IUserRepository {
private:
    // Instância Singleton
    static NvsUserRepository* instance;
    
    // Configuração do NVS
    const std::string nvs_namespace = "users";      // Namespace principal para dados
    const std::string index_namespace = "user_idx"; // Namespace para índices
    nvs_handle_t nvs_handle;                        // Handle para dados dos usuários
    nvs_handle_t index_handle;                      // Handle para índices
    bool is_initialized;
    
    // Construtor privado para aplicar o padrão Singleton
    NvsUserRepository();
    
    // Métodos auxiliares para serialização (Factory Method)
    domain::entities::User userFromJson(const std::string& json_string) const;
    std::string userToJson(const domain::entities::User& user) const;
    
    // Métodos auxiliares para gerenciamento de índices
    std::string createProviderIndex(const std::string& provider, 
                                   const std::string& providerId) const;
    std::string createEmailIndex(const std::string& email) const;
    
    // Inicialização interna
    void initialize();

public:
    // Método de acesso Singleton
    static NvsUserRepository* getInstance();
    
    // Destrutor
    ~NvsUserRepository();
    
    // Deletando construtor de cópia e operador de atribuição (para manter Singleton)
    NvsUserRepository(const NvsUserRepository&) = delete;
    NvsUserRepository& operator=(const NvsUserRepository&) = delete;
    
    // Implementação da interface IUserRepository
    std::optional<domain::entities::User> findByProviderId(
        const std::string& provider, 
        const std::string& providerId) override;
    std::optional<domain::entities::User> findByEmail(const std::string& email) override;
    void save(const domain::entities::User& user) override;
    
    // Operações CRUD adicionais
    std::optional<domain::entities::User> findById(const std::string& id);
    bool deleteUser(const std::string& id);
    std::vector<domain::entities::User> findAll();
};

} // namespace infrastructure::persistence
