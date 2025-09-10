#pragma once

#include <string>
#include <chrono>

namespace domain::entities {

class User {
private:
    std::string id;
    std::string macAddress;
    std::string name;
    std::string email;
    std::string provider;
    std::string providerId;
    std::chrono::system_clock::time_point lastLogin;

    User(std::string id, std::string macAddress, std::string name, std::string email, 
         std::string provider, std::string providerId);

public:
    static User create(const std::string& macAddress, const std::string& name, const std::string& email,
                       const std::string& provider, const std::string& providerId);

    const std::string& getId() const;
    const std::string& getMacAddress() const;
    const std::string& getName() const;
    const std::string& getEmail() const;
    const std::string& getProvider() const;
    const std::string& getProviderId() const;

    void updateLastLogin();
    void updateProfile(const std::string& newName, const std::string& newEmail);
};

}