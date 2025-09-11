#pragma once

#include <string>
#include <chrono>

namespace domain::entities {

class User {
private:
    std::string id;
    std::string name;
    std::string email;
    std::string provider;
    std::string providerId;
    std::chrono::system_clock::time_point lastLogin;

    // Construtor privado para forçar a criação através do método estático "create"
    User(std::string id, std::string name, std::string email, 
         std::string provider, std::string providerId);

public:
    // Factory Method: Garante que um User só pode ser criado em um estado válido
    static User create(const std::string& id, const std::string& name, const std::string& email,
                       const std::string& provider, const std::string& providerId);

    // Getters
    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getEmail() const;
    const std::string& getProvider() const;
    const std::string& getProviderId() const;
    const std::chrono::system_clock::time_point& getLastLogin() const;

    // Métodos com lógica de negócio
    void updateLastLogin();
    void updateProfile(const std::string& newName, const std::string& newEmail);
};

} // namespace domain::entities