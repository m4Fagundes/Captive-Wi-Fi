#pragma once


#include "domain/repositories/IUserRepository.h"
using domain::repositories::IUserRepository;

#include "domain/entities/User.h"
#include <string>

namespace domain::services {

// DTO (Data Transfer Object) para passar os dados do perfil OAuth de forma limpa
struct OAuthProfile {
    std::string provider;
    std::string providerId;
    std::string email;
    std::string name;
};

class OAuthUserService {
private:
    IUserRepository& userRepository;

public:
    explicit OAuthUserService(IUserRepository& repository);

    // Lógica de negócio principal: "Encontre ou Crie" o usuário
    entities::User findOrCreateUser(const OAuthProfile& profile);
};

} // namespace domain::services