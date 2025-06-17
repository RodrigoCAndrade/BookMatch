/**
 * @file: user.cpp
 * @author: Rodrigo Andrade
 * @date: 16 Jun 2025
 * @description: Implementação da classe User.
 * @version: 2.0
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
*/

#include "user.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <filesystem>
#include <openssl/evp.h>
#include <openssl/sha.h>

/**
 * @brief Construtor principal da classe User.
 * @param dataManager Uma referência ao gerenciador de dados.
 */
User::User(DataManager& dataManager) : dataManager(dataManager) {}

// --- Getters & Setters ---

std::string User::getUsername() const { return this->username; }
void User::setUsername(const std::string& username) { this->username = username; }
std::string User::getPasswordHash() const { return this->passwordHash; }
void User::setPasswordHash(const std::string& hash) { this->passwordHash = hash; }

/**
 * @brief Gera o hash de uma senha em texto plano usando SHA-512.
 * @param plainPassword A senha a ser processada.
 */
void User::hashPassword(const std::string& plainPassword) {
    unsigned char hash[SHA512_DIGEST_LENGTH];
    EVP_MD_CTX* context = EVP_MD_CTX_new();

    if (!context || !EVP_DigestInit_ex(context, EVP_sha512(), NULL) ||
        !EVP_DigestUpdate(context, plainPassword.c_str(), plainPassword.length())) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Falha ao inicializar o contexto de hash SHA-512.");
    }

    unsigned int lengthOfHash = 0;
    if (!EVP_DigestFinal_ex(context, hash, &lengthOfHash)) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Falha ao finalizar o hash SHA-512.");
    }

    EVP_MD_CTX_free(context);

    std::stringstream ss;
    for (unsigned int i = 0; i < lengthOfHash; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    this->passwordHash = ss.str();
}

/**
 * @brief Verifica se um usuário com o 'username' atual existe no arquivo.
 * @return true se o usuário existe, false caso contrário.
 */
bool User::exists() {
    if (!dataManager.open(std::ios::in)) {
        return false;
    }

    std::string line;
    bool found = false;
    while (dataManager.getLine(line)) {
        size_t pos = line.find(':');
        if (pos != std::string::npos && line.substr(0, pos) == this->username) {
            found = true;
            break;
        }
    }
    dataManager.close();
    return found;
}

/**
 * @brief Carrega os dados do usuário do arquivo para este objeto.
 * @return true se o usuário foi encontrado e carregado, false caso contrário.
 */
bool User::load() {
    if (!dataManager.open(std::ios::in)) {
        return false;
    }

    std::string line;
    bool found = false;
    while (dataManager.getLine(line)) {
        size_t pos = line.find(':');
        if (pos != std::string::npos && line.substr(0, pos) == this->username) {
            this->passwordHash = line.substr(pos + 1);
            found = true;
            break;
        }
    }
    dataManager.close();
    return found;
}

/**
 * @brief Salva os dados do usuário (username e hash da senha) no arquivo.
 * @return true se a operação foi bem-sucedida, false caso contrário.
 */
bool User::save() {
    std::string originalPath = dataManager.getFullPath();
    std::string tempPath = originalPath + ".tmp";

    std::ofstream tempFile(tempPath);
    if (!tempFile.is_open()) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo temporario." << std::endl;
        return false;
    }

    std::ifstream originalFile(originalPath);
    if (originalFile.is_open()) {
        std::string line;
        while (std::getline(originalFile, line)) {
            size_t pos = line.find(':');
            if (pos == std::string::npos || line.substr(0, pos) != this->username) {
                tempFile << line << std::endl;
            }
        }
    }
    originalFile.close();

    tempFile << this->username << ":" << this->passwordHash << std::endl;
    tempFile.close();

    try {
        std::filesystem::rename(tempPath, originalPath);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Erro de Filesystem ao renomear: " << e.what() << std::endl;
        return false;
    }

    return true;
}
