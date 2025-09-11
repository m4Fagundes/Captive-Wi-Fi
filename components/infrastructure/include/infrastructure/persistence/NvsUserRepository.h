// #pragma once

// #include "domain/repositories/IUserRepository.h" // Inclui a interface que estamos implementando
// #include "nvs.h" // Inclui o header do NVS para usarmos o nvs_handle_t
// #include <string>

// namespace infrastructure::persistence {

// /**
//  * @brief Implementação concreta de IUserRepository usando o NVS (Non-Volatile Storage) do ESP32.
//  * Esta classe é responsável por serializar a entidade User e salvá-la na memória flash.
//  */
// class NvsUserRepository : public domain::repositories::IUserRepository {
// private:
//     const std::string nvs_namespace = "users"; // Namespace no NVS para isolar nossos dados
//     nvs_handle_t nvs_handle; // Handle para o armazenamento NVS

//     // Função helper para deserializar um JSON e criar um objeto User
//     domain::entities::User deserialize(const std::string& json_string) const;

// public:
//     NvsUserRepository();  // Construtor para inicializar o NVS
//     ~NvsUserRepository(); // Destrutor para fechar o handle do NVS

//     // Sobrescreve (override) os métodos da interface
//     std::optional<domain::entities::User> findByProviderId(const std::string& provider, const std::string& providerId) override;
//     std::optional<domain::entities::User> findByEmail(const std::string& email) override;
//     void save(const domain::entities::User& user) override;
// };

// } // namespace infrastructure::persistence