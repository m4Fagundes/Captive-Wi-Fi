#include "User.h"
#include <stdexcept>
#include <uuid/uuid.h>

namespace domain::entities {

User::User(std::string id, std::string macAddress, std::string name, std::string email,
           std::string provider, std::string providerId)
    : id(std::move(id)), macAddress(std::move(macAddress)), name(std::move(name)), 
      email(std::move(email)), provider(std::move(provider)), providerId(std::move(providerId)) {
    this->lastLogin = std::chrono::system_clock::now();
}

User User::create(const std::string& macAddress, const std::string& name, const std::string& email,
                  const std::string& provider, const std::string& providerId) {
    if (macAddress.empty() || provider.empty() || providerId.empty() || email.empty()) {
        throw std::invalid_argument("Dados do perfil OAuth ou MAC Address estão faltando.");
    }

    uuid_t uuid;
    uuid_generate_random(uuid);
    char uuid_str[37];
    uuid_unparse_lower(uuid, uuid_str);

    return User(uuid_str, macAddress, name, email, provider, providerId);
}

const std::string& User::getId() const { return id; }
const std::string& User::getMacAddress() const { return macAddress; }
const std::string& User::getName() const { return name; }
const std::string& User::getEmail() const { return email; }
const std::string& User::getProvider() const { return provider; }
const std::string& User::getProviderId() const { return providerId; }

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

}