cmake_minimum_required(VERSION 3.10)
project(BookMatch)

# Encontrar o OpenSSL
find_package(OpenSSL REQUIRED)

# Adicionar os diretórios de include
include_directories(${OPENSSL_INCLUDE_DIR})

# Adicionar os arquivos fonte
set(SOURCES
    src/main.cpp
    src/user.cpp
    src/data.cpp
)

# Criar o executável
add_executable(BookMatch ${SOURCES})

# Linkar com OpenSSL
target_link_libraries(BookMatch ${OPENSSL_LIBRARIES})
