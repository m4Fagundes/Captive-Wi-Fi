#pragma once

#include "../interfaces/IUserRepository.h"
#include "../entities/User.h"
#include <string>

namespace domain::services {

struct OAuthProfile {
    std::string provider;
    std::string providerId;
    std::string email;
    std::string name;
};

class OAuthUserService {
private:
    interfaces::IUserRepository& userRepository;

public:
    explicit OAuthUserService(interfaces::IUserRepository& repository);

    entities::User findOrCreateUser(const OAuthProfile& profile, const std::string& macAddress);
};

}