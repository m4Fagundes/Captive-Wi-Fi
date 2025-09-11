#include "domain/service/OAuthUserService.h"

#include <chrono>   // Para std::chrono
#include <string>   // Para std::to_string
#include <cstdlib>  // Para rand()

namespace domain::services {

OAuthUserService::OAuthUserService(IUserRepository& repository)
    : userRepository(repository) {}

entities::User OAuthUserService::findOrCreateUser(const OAuthProfile& profile) {
    auto userOpt = userRepository.findByProviderId(profile.provider, profile.providerId);

    if (userOpt) {
        // Usuário EXISTE: atualiza dados, salva e retorna
        auto& existingUser = *userOpt;
        existingUser.updateProfile(profile.name, profile.email);
        existingUser.updateLastLogin();
        userRepository.save(existingUser);
        return existingUser;
    } else {
        // Usuário NÃO EXISTE: cria um novo, salva e retorna
        
        // --- Geração de ID Único (Placeholder) ---
        // TODO: Substituir por um gerador de UUID real da camada de Infraestrutura
        long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
        std::string newId = profile.provider + "-" + std::to_string(timestamp);
        // -----------------------------------------

        auto newUser = entities::User::create(
            newId,
            profile.name,
            profile.email,
            profile.provider,
            profile.providerId
        );
        userRepository.save(newUser);
        return newUser;
    }
}

} // namespace domain::services