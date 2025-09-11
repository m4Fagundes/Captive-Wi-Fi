# CMakeLists.txt para o componente 'domain'

idf_component_register(
    # Lista de todos os arquivos de implementação (.cpp) do componente
    SRCS 
        "User.cpp"
        "OAuthUserService.cpp"

    # Diretórios de include que este componente torna públicos para outros
    INCLUDE_DIRS 
        "include"

    # Lista de outros componentes dos quais este componente depende
    # Essencial: A camada de Domínio não depende de NENHUM outro componente.
    REQUIRES 
        ""
)