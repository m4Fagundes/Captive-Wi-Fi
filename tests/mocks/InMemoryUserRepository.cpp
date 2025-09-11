#include "InMemoryUserRepository.h"
#include <iostream>

namespace host_infrastructure {

void InMemoryUserRepository::save(const domain::entities::User& user) {
    std::cout << "[Host DB] Salvando usuário: " << user.getName() 
              << " (ID: " << user.getId() << ")" << std::endl;
    
    // CORREÇÃO: Usando insert_or_assign para evitar o construtor padrão
    users_db.insert_or_assign(user.getId(), user);
}

std::optional<domain::entities::User> InMemoryUserRepository::findByProviderId(const std::string& provider, const std::string& providerId) {
    std::cout << "[Host DB] Buscando por Provider ID: " << provider << "-" << providerId << std::endl;
    for (const auto& pair : users_db) {
        if (pair.second.getProvider() == provider && pair.second.getProviderId() == providerId) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::optional<domain::entities::User> InMemoryUserRepository::findByEmail(const std::string& email) {
    std::cout << "[Host DB] Buscando por Email: " << email << std::endl;
    for (const auto& pair : users_db) {
        if (pair.second.getEmail() == email) {
            return pair.second;
        }
    }
    return std::nullopt;
}

} // namespace host_infrastructure