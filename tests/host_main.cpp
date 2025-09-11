#include <iostream>
#include "domain/service/OAuthUserService.h"
#include "mocks/InMemoryUserRepository.h" // Usa nossa implementação fake

// Ponto de entrada para rodar na sua máquina (Mac/Linux)
int main() {
    std::cout << "--- Iniciando Teste no Host ---" << std::endl;

    // 1. Instancia a infraestrutura FAKE
    host_infrastructure::InMemoryUserRepository userRepo;

    // 2. Instancia o serviço de domínio, injetando a dependência fake
    domain::services::OAuthUserService oauthService(userRepo);

    // 3. Simula um perfil de usuário retornado pelo Google
    domain::services::OAuthProfile googleProfile;
    googleProfile.provider = "google";
    googleProfile.providerId = "123456789";
    googleProfile.name = "Matheus Fagundes";
    googleProfile.email = "matheus.fagundes@example.com";

    // 4. Executa o caso de uso "Find or Create" pela primeira vez
    std::cout << "\n[APP] Primeira chamada para findOrCreateUser..." << std::endl;
    auto user1 = oauthService.findOrCreateUser(googleProfile);
    std::cout << "[APP] Usuário processado: " << user1.getName() << std::endl;

    // 5. Executa pela segunda vez para simular um usuário que já existe
    std::cout << "\n[APP] Segunda chamada para findOrCreateUser..." << std::endl;
    auto user2 = oauthService.findOrCreateUser(googleProfile);
    std::cout << "[APP] Usuário processado: " << user2.getName() << std::endl;

    std::cout << "\n--- Teste no Host Concluído ---" << std::endl;

    return 0;
}