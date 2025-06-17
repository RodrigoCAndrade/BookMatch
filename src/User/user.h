/**
 * @file: user.h
 * @author: Rodrigo Andrade
 * @date: 16 Jun 2025
 * @description: Definição da classe User para gerenciar dados e autenticação de usuários.
 * @version: 2.0
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
*/

#ifndef USER_H
#define USER_H

#include <string>
#include "../Data/data_manager.h"

/**
 * @class User
 * @brief Gerencia as informações e operações de um usuário.
 *
 * Responsável por armazenar dados do usuário, gerar hashes de senha e
 * persistir as informações usando um DataManager.
 */
class User {
private:
    std::string username;
    std::string passwordHash;
    DataManager& dataManager;

public:
    /**
     * @brief Construtor principal para um objeto User.
     * @param dataManager Uma referência ao gerenciador de dados para persistência.
     */
    explicit User(DataManager& dataManager);

    /**
     * @brief Gera o hash de uma senha em texto plano usando SHA-512.
     * @param plainPassword A senha a ser processada.
     * @note Por segurança máxima, considere usar Argon2 ou scrypt em vez de SHA-512.
     */
    void hashPassword(const std::string& plainPassword);

    /**
     * @brief Salva os dados do usuário (username e hash da senha) no arquivo.
     * @return true se a operação foi bem-sucedida, false caso contrário.
     */
    bool save();

    /**
     * @brief Carrega os dados do usuário do arquivo para este objeto.
     * @return true se o usuário foi encontrado e carregado, false caso contrário.
     */
    bool load();

    /**
     * @brief Verifica se um usuário com o 'username' atual existe no arquivo.
     * @return true se o usuário existe, false caso contrário.
     */
    bool exists();

    // --- Getters & Setters ---
    std::string getUsername() const;
    void setUsername(const std::string& username);
    std::string getPasswordHash() const;
    void setPasswordHash(const std::string& hash);
};

#endif // USER_H
