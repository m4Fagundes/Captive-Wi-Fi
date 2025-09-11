# Captive Portal para ESP32 com Login OAuth

![ESP32](https-media.wired.com-photos-5926818a7034dc5f91bec26d-master-w_2560%2Cc_limit-ESP32-Chip-TA.jpg)

Este projeto implementa um portal cativo (_captive portal_) para a plataforma ESP32, utilizando C++ e os princípios da **Clean Architecture**. O objetivo é criar um sistema robusto, testável e modular que força os usuários de uma rede Wi-Fi a se autenticarem usando suas contas do Google (OAuth 2.0) antes de liberar o acesso à internet.

O projeto é construído com uma arquitetura híbrida, permitindo que toda a lógica de negócio seja desenvolvida e testada em uma máquina host (macOS/Linux) antes de ser embarcada no hardware do ESP32, garantindo um ciclo de desenvolvimento rápido e eficiente.

## Funcionalidades

- **Portal Cativo**: Intercepta todo o tráfego de novos dispositivos conectados à rede.
- **Access Point Wi-Fi**: O próprio ESP32 cria a rede Wi-Fi para os usuários se conectarem.
- **Autenticação OAuth 2.0**: Integração com o fluxo de login do Google para uma autenticação segura e sem senhas.
- **Arquitetura Limpa**: Separação clara entre a lógica de negócio (Domínio), as regras da aplicação (Aplicação) e os detalhes de hardware/serviços (Infraestrutura).
- **Desenvolvimento Híbrido**: Capacidade de compilar e testar a lógica principal em um computador local sem a necessidade do hardware.
- **Persistência de Dados**: Armazenamento de informações dos usuários na memória flash do ESP32 utilizando o NVS (Non-Volatile Storage).

## Arquitetura do Projeto

O código é organizado em camadas, seguindo os princípios da **Clean Architecture**, para garantir baixo acoplamento e alta coesão.

### 1. Camada de Domínio (`components/domain`)

O coração do sistema. Contém a lógica de negócio pura, sem nenhuma dependência do hardware do ESP32 ou de frameworks externos.

- **`User.h / User.cpp` (Entidade)**: Representa um usuário do sistema. Contém os dados (ID, nome, email, etc.) e as regras de negócio intrínsecas a um usuário (ex: `updateProfile`, `updateLastLogin`).

- **`IUserRepository.h` (Interface de Repositório)**: Define o **contrato** para a persistência de dados. A camada de Domínio não sabe *como* os dados são salvos, apenas *quais* operações de salvamento e busca são necessárias.

- **`OAuthUserService.h / OAuthUserService.cpp` (Serviço de Domínio)**: Orquestra a lógica de negócio que envolve múltiplas entidades ou regras mais complexas. Sua principal responsabilidade é o caso de uso "Encontrar ou Criar Usuário" (`findOrCreateUser`) com base no perfil retornado pelo Google.

### 2. Camada de Infraestrutura (`components/infrastructure`)

A implementação dos detalhes técnicos. É a ponte entre a lógica de negócio e o mundo real (hardware, serviços externos).

- **`NvsUserRepository.h / NvsUserRepository.cpp` (Repositório)**: Implementação concreta da interface `IUserRepository`. É responsável por salvar e ler os dados da entidade `User` na memória flash do ESP32 (NVS), utilizando serialização em JSON.

- **`WifiManager.h / WifiManager.cpp` (Gerenciador de Rede)**: Responsável por configurar e iniciar o ESP32 no modo Access Point (AP), criando a rede Wi-Fi para os usuários.

- **`GoogleOAuthClient.h / GoogleOAuthClient.cpp` (Cliente HTTP)**: Responsável por realizar as chamadas HTTPS para os servidores do Google, trocando o código de autorização pelo perfil do usuário.

### 3. Camada de Aplicação (`main/application`)

Contém os casos de uso específicos que orquestram o fluxo da aplicação, conectando a infraestrutura ao domínio. Exemplo: `HandleGoogleCallbackUseCase`, que coordena o recebimento do callback do Google e chama o `OAuthUserService`.

## Desenvolvimento e Teste no Host

Para acelerar o desenvolvimento, o projeto pode ser compilado e executado em um computador local. Para isso, utilizamos uma implementação "fake" da infraestrutura:

- **`InMemoryUserRepository.h / InMemoryUserRepository.cpp` (Mock)**: Uma implementação de `IUserRepository` que salva os dados em um mapa na memória RAM, permitindo testar todo o fluxo da camada de Domínio sem depender do hardware do ESP32.

## Como Compilar

### Na Máquina Host (para Testes Rápidos)

```bash
# Na raiz do projeto
mkdir build_host
cd build_host

# Configura o projeto
cmake ..

# Compila
make

# Executa o programa de teste
./run_host_tests
