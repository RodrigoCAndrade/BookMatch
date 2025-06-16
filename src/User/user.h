/**
 * @file user.h
 * @brief Classe responsável pelo gerenciamento de usuários do sistema
 * @author Rodrigo Andrade
 * @date 2025
 * @version 1.0
 * @license MIT
 */

#ifndef USER_H
#define USER_H

#include <string>
#include "../Data/data.h"

using namespace std;

/**
 * @class User
 * @brief Gerencia informações e operações relacionadas aos usuários
 *
 * Esta classe é responsável por:
 * - Armazenar dados do usuário (nome e senha)
 * - Gerenciar autenticação
 * - Persistir dados do usuário
 */
class User {
    private:
        string username;    // Nome do usuário
        string password;    // Senha criptografada do usuário
        Data& data;        // Referência para o gerenciador de dados

    public:
        /**
         * @brief Construtor da classe User
         * @param username Nome do usuário
         * @param data Gerenciador de dados
         */
        User(string username, Data& data);

        /**
         * @brief Obtém o gerenciador de dados
         * @return Referência para o gerenciador de dados
         */
        Data& getData();

        /**
         * @brief Obtém o nome do usuário
         * @return Nome do usuário
         */
        string getUsername();

        /**
         * @brief Define o nome do usuário
         * @param username Novo nome do usuário
         */
        void setUsername(string username);

        /**
         * @brief Obtém a senha do usuário
         * @return Senha criptografada
         */
        string getPassword();

        /**
         * @brief Define a senha do usuário
         * @param password Nova senha
         */
        void setPassword(string password);

        /**
         * @brief Criptografa a senha usando SHA-512
         * @param password Senha a ser criptografada
         */
        void hashPassword(string password);

        /**
         * @brief Salva os dados do usuário
         * @return 1 se salvou com sucesso, 0 caso contrário
         */
        int saveUser();

        /**
         * @brief Verifica se o usuário existe
         * @return 1 se existe, 0 se não existe
         */
        int hasUser();

        /**
         * @brief Carrega os dados do usuário
         * @return 1 se carregou com sucesso, 0 caso contrário
         */
        int loadUser();
};

#endif
