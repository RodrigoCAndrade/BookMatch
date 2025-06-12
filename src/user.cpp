/**
 * @file user.cpp
 * @brief Implementação da classe User para gerenciamento de usuários
 * @author Rodrigo Andrade
 * @date 11.06.2025
 * @version 1.0
 * @license MIT
 */

#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include "user.h"
#include "data.h"

/**
 * @brief Construtor da classe User
 * @param username Nome do usuário
 */
User::User(string username) : username(username) {
}

/**
 * @brief Obtém o nome do usuário
 * @return Nome do usuário
 */
string User::getUsername() {
    return User::password;
}

/**
 * @brief Obtém a senha do usuário
 * @return Senha criptografada
 */
string User::getPassword() {
    return User::password;
}

/**
 * @brief Define o nome do usuário
 * @param username Novo nome de usuário
 */
void User::setUsername(string username) {
    User::username = username;
}

/**
 * @brief Define a senha do usuário
 * @param password Nova senha
 */
void User::setPassword(string password) {
    User::password = password;
}

/**
 * @brief Criptografa a senha usando SHA-512
 * @param password Senha a ser criptografada
 *
 * A senha é criptografada usando o algoritmo SHA-512 e armazenada
 * em formato hexadecimal.
 */
void User::hashPassword(string password) {
    // Buffer para armazenar o hash SHA-512 (64 bytes)
    unsigned char hash[SHA512_DIGEST_LENGTH];

    // Cria o contexto SHA-512
    SHA512_CTX sha512;

    // Inicializa o contexto SHA-512
    SHA512_Init(&sha512);

    // Adiciona a senha ao contexto para ser processada
    SHA512_Update(&sha512, password.c_str(), password.length());

    // Finaliza o cálculo do hash e armazena o resultado no buffer 'hash'
    SHA512_Final(hash, &sha512);

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
 * @param data Referência para o gerenciador de dados
 * @return 1 se o usuário existe, 0 caso contrário
 */
int User::hasUser(Data& data) {
    string filename = data.getUserDataFilename();

    if (data.openFile(filename, ios::in)) {
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
 * @param data Referência para o gerenciador de dados
 * @return 1 se os dados foram carregados com sucesso, 0 caso contrário
 */
int User::loadUser(Data& data) {
    string filename = data.getUserDataFilename();

    if (data.openFile(filename, ios::in)) {
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
 * @param data Referência para o gerenciador de dados
 * @return 1 se os dados foram salvos com sucesso, 0 caso contrário
 *
 * O método mantém os dados de outros usuários intactos e
 * atualiza apenas os dados do usuário atual.
 */
int User::saveUser(Data& data) {
    string filename = data.getUserDataFilename();

    // Primeiro, lê todos os usuários existentes
    vector<string> users;
    if (data.openFile(filename, ios::in)) {
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
    if (data.openFile(filename, ios::out)) {
        for (const string& user : users) {
            data.writeToFile(user + "\n");
        }
        data.closeFile();
        return 1;
    }
    return 0;
}
