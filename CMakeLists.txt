cmake_minimum_required(VERSION 3.12)
project(BookMatch LANGUAGES CXX)

# Padrão o C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Constroi OPENSSL como estático
set(OPENSSL_USE_STATIC_LIBS TRUE)

# Busca o OpenSSL
find_package(OpenSSL REQUIRED)

# Adiciona os diretórios de include
include_directories(${OPENSSL_INCLUDE_DIR})

# Adiciona os arquivos fonte
set(SOURCES
    src/main.cpp
    src/User/user.cpp
    src/Data/data_manager.cpp
    src/Book/book.cpp
)

# Cria o executável
add_executable(BookMatch ${SOURCES})

# Linka com OpenSSL
target_link_libraries(BookMatch OpenSSL::SSL OpenSSL::Crypto)
