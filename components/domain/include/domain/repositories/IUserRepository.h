#pragma once

#include "domain/entities/User.h"
#include <string>
#include <optional>

namespace domain::repositories {

/**
 * @brief Interface (contrato) para um repositório de usuários.
 * Define as operações de persistência necessárias para a camada de Domínio,
 * sem se preocupar com a tecnologia de armazenamento (Flash, SD Card, etc.).
 */
class IUserRepository {
public:
    // Destrutor virtual é obrigatório para interfaces em C++
    virtual ~IUserRepository() = default;

    // Métodos puros virtuais que devem ser implementados pela camada de Infraestrutura
    virtual std::optional<entities::User> findByProviderId(const std::string& provider, const std::string& providerId) = 0;
    virtual std::optional<entities::User> findByEmail(const std::string& email) = 0;
    virtual void save(const entities::User& user) = 0;
};

} // namespace domain::repositories