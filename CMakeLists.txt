cmake_minimum_required(VERSION 3.20)
project(BookMatch LANGUAGES CXX)

include(FetchContent)

# Padrão o C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define o diretório de saída para o executável
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


# 1. nlohmann_json (header-only)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3 # Uma versão estável popular
)
FetchContent_MakeAvailable(nlohmann_json)

# 2. botan
find_package(botan REQUIRED)

# 3. tabulate (header-only)
FetchContent_Declare(
    tabulate
    GIT_REPOSITORY https://github.com/p-ranav/tabulate.git
    GIT_TAG        v1.5
)
FetchContent_MakeAvailable(tabulate)

# --- Fim das Dependências ---

# Adiciona os arquivos fonte do seu projeto
add_executable(BookMatch
    src/Main.cpp
    src/Book/Book.cpp
    src/DataManager/DataManager.cpp
    src/User/User.cpp
    src/Utils/FormatAux.cpp
)

# Adiciona os diretórios 'src' para includes
target_include_directories(BookMatch PUBLIC
    src
    ${nlohmann_json_SOURCE_DIR}/include
)

# Linka o executável com todas as bibliotecas
target_link_libraries(BookMatch PRIVATE
    nlohmann_json::nlohmann_json
    Botan::Botan-static
    tabulate::tabulate
)
