/**
 * @file user.cpp
 * @brief Implementação da classe User para gerenciamento de usuários
 * @author Rodrigo Andrade
 * @date 11.06.2025
 * @version 1.0
 * @license MIT
 */

#include <iostream>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include "../User/user.h"
#include "../Data/data.h"

/**
 * @brief Construtor da classe User
 * @param username Nome do usuário
 * @param data Gerenciador de dados
 */
User::User(string username, Data& data) : username(username), data(data) {
}

/**
 * @brief Obtém o gerenciador de dados
 * @return Referência para o gerenciador de dados
 */
Data& User::getData() {
    return data;
}

/**
 * @brief Obtém o nome do usuário
 * @return Nome do usuário
 */
string User::getUsername() {
    return username;
}

/**
 * @brief Obtém a senha do usuário
 * @return Senha criptografada
 */
string User::getPassword() {
    return password;
}

/**
 * @brief Define o nome do usuário
 * @param username Novo nome de usuário
 */
void User::setUsername(string username) {
    this->username = username;
}

/**
 * @brief Define a senha do usuário
 * @param password Nova senha
 */
void User::setPassword(string password) {
    this->password = password;
}

/**
 * @brief Criptografa a senha usando SHA-512
 * @param password Senha a ser criptografada
 */
void User::hashPassword(string password) {
    // Buffer para armazenar o hash SHA-512 (64 bytes)
    unsigned char hash[SHA512_DIGEST_LENGTH];

    // Cria o contexto para o hash
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw runtime_error("Failed to create hash context");
    }

    // Inicializa o contexto com SHA-512
    if (!EVP_DigestInit_ex(ctx, EVP_sha512(), NULL)) {
        EVP_MD_CTX_free(ctx);
        throw runtime_error("Failed to initialize hash context");
    }

    // Adiciona a senha ao contexto para ser processada
    if (!EVP_DigestUpdate(ctx, password.c_str(), password.length())) {
        EVP_MD_CTX_free(ctx);
        throw runtime_error("Failed to update hash context");
    }

    // Finaliza o cálculo do hash e armazena o resultado no buffer 'hash'
    unsigned int len;
    if (!EVP_DigestFinal_ex(ctx, hash, &len)) {
        EVP_MD_CTX_free(ctx);
        throw runtime_error("Failed to finalize hash");
    }

    // Libera o contexto
    EVP_MD_CTX_free(ctx);

    // Converte o hash binário para uma string hexadecimal
    std::stringstream ss;
    for(int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        // Converte cada byte para hexadecimal com 2 dígitos
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    // Armazena a senha criptografada
    setPassword(ss.str());
}

/**
 * @brief Verifica se o usuário existe no sistema
 * @return 1 se o usuário existe, 0 caso contrário
 */
int User::hasUser() {
    if (data.openFile(ios::in)) {
        string line;
        while (data.getLine(line)) {
            size_t pos = line.find(':');
            if (pos != string::npos) {
                string storedUsername = line.substr(0, pos);
                if (storedUsername == username) {
                    data.closeFile();
                    return 1;
                }
            }
        }
        data.closeFile();
    }
    return 0;
}

/**
 * @brief Carrega os dados do usuário do arquivo
 * @return 1 se os dados foram carregados com sucesso, 0 caso contrário
 */
int User::loadUser() {
    if (data.openFile(ios::in)) {
        string line;
        while (data.getLine(line)) {
            size_t pos = line.find(':');
            if (pos != string::npos) {
                string storedUsername = line.substr(0, pos);
                if (storedUsername == username) {
                    password = line.substr(pos + 1);
                    data.closeFile();
                    return 1;
                }
            }
        }
        data.closeFile();
    }
    return 0;
}

/**
 * @brief Salva os dados do usuário no arquivo
 * @return 1 se os dados foram salvos com sucesso, 0 caso contrário
 *
 * O método mantém os dados de outros usuários intactos e
 * atualiza apenas os dados do usuário atual.
 */
int User::saveUser() {
    // Primeiro, lê todos os usuários existentes
    vector<string> users;
    if (data.openFile(ios::in)) {
        string line;
        while (data.getLine(line)) {
            size_t pos = line.find(':');
            if (pos != string::npos) {
                string storedUsername = line.substr(0, pos);
                if (storedUsername != username) {
                    users.push_back(line);
                }
            }
        }
        data.closeFile();
    }

    // Adiciona o usuário atual
    users.push_back(username + ":" + password);

    // Salva todos os usuários de volta
    if (data.openFile(ios::out)) {
        for (const string& user : users) {
            data.writeToFile(user + "\n");
        }
        data.closeFile();
        return 1;
    }
    return 0;
}
