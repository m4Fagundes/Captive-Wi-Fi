#include "domain/entities/User.h"
#include <stdexcept>

namespace domain::entities {

User::User(std::string id, std::string name, std::string email,
           std::string provider, std::string providerId)
    : id(std::move(id)), name(std::move(name)), email(std::move(email)), 
      provider(std::move(provider)), providerId(std::move(providerId)) {
    this->lastLogin = std::chrono::system_clock::now();
}

User User::create(const std::string& id, const std::string& name, const std::string& email,
                  const std::string& provider, const std::string& providerId) {
    // Validações para garantir a consistência da entidade
    if (id.empty() || provider.empty() || providerId.empty() || email.empty()) {
        throw std::invalid_argument("Dados do perfil ou ID estão faltando para criar o usuário.");
    }
    return User(id, name, email, provider, providerId);
}

// Implementação dos Getters
const std::string& User::getId() const { return id; }
const std::string& User::getName() const { return name; }
const std::string& User::getEmail() const { return email; }
const std::string& User::getProvider() const { return provider; }
const std::string& User::getProviderId() const { return providerId; }
const std::chrono::system_clock::time_point& User::getLastLogin() const { return lastLogin; }

// Implementação dos métodos de negócio
void User::updateLastLogin() {
    this->lastLogin = std::chrono::system_clock::now();
}

void User::updateProfile(const std::string& newName, const std::string& newEmail) {
    if (!newName.empty()) {
        this->name = newName;
    }
    if (!newEmail.empty()) {
        this->email = newEmail;
    }
}

} // namespace domain::entities