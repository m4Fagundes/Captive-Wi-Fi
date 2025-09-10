App.cpp: Inicializa a aplicação e orquestra os módulos do domínio.

User: Representa um usuário da rede cativa.

IUserRepository / InMemoryUserRepository: Abstração e implementação de armazenamento de usuários em memória.

OAuthUserService: Serviço responsável pela autenticação via Google OAuth e gestão de sessões.

portal.h: Header principal para integração do módulo portal.

web/: Contém a interface web onde o usuário acessa a rede cativa e realiza login.

shared/: Utilitários ou componentes compartilhados entre diferentes módulos.

tests/: Contém testes automatizados para garantir que o sistema funciona corretamente.
