# CMakeLists.txt para o componente 'infrastructure'

idf_component_register(
    SRCS
        "persistence/NvsUserRepository.cpp"

    INCLUDE_DIRS
        "include"

    # Declara que este componente DEPENDE dos componentes a seguir.
    # O sistema de build garante que os includes e bibliotecas deles
    # estarão disponíveis para nós.
    REQUIRES
        "domain"        # Para acessar as interfaces como IUserRepository
        "nvs_flash"     # Para usar o sistema de armazenamento NVS
        "cjson"         # Para serializar/desserializar objetos para JSON
)