#pragma once

#include "domain/repositories/IUserRepository.h"
#include <map>
#include <string>

namespace host_infrastructure {

/**
 * @brief Implementação de IUserRepository que funciona em memória.
 * Perfeita para testes rápidos no computador local, sem depender do ESP32.
 */
class InMemoryUserRepository : public domain::repositories::IUserRepository {
private:
    // Um mapa para simular o banco de dados. A chave é o ID do usuário.
    std::map<std::string, domain::entities::User> users_db;

public:
    InMemoryUserRepository() = default;
    ~InMemoryUserRepository() = default;

    std::optional<domain::entities::User> findByProviderId(const std::string& provider, const std::string& providerId) override;
    std::optional<domain::entities::User> findByEmail(const std::string& email) override;
    void save(const domain::entities::User& user) override;
};

} // namespace host_infrastructure