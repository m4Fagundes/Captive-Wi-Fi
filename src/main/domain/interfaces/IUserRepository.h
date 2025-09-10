#pragma once

#include "../entities/User.h"
#include <string>
#include <optional>

namespace domain::interfaces {

class IUserRepository {
public:
    virtual ~IUserRepository() = default;

    virtual std::optional<entities::User> findByProviderId(const std::string& provider, const std::string& providerId) = 0;
    
    virtual std::optional<entities::User> findByEmail(const std::string& email) = 0;

    virtual std::optional<entities::User> findByMacAddress(const std::string& macAddress) = 0;
    
    virtual void save(const entities::User& user) = 0;
};

}