/**
 * @file: main.cpp
 * @author: Rodrigo Andrade
 * @date: 16 Jun 2025
 * @description: Classe principal para a aplicação BookMatch.
 * @version: 2.0
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
*/


#include <iostream>
#include <string>
#include <sstream>
#include "Book/book.h"
#include "User/user.h"
#include "Data/data_manager.h"

// Bloco para configurar o terminal do Windows para aceitar caracteres UTF-8
#ifdef _WIN32
#include <windows.h>
#endif

// --- Constantes para Cores ANSI ---
// Usadas para formatar a saída do console de forma portável.
const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string CYAN = "\033[36m";

/**
 * @brief Configura o console para a saída de caracteres UTF-8.
 * @note Esta função é específica para o sistema operacional Windows.
 */
void setupConsole();

/**
 * @brief Exibe o menu principal com a arte ASCII do BookMatch.
 */
void displayMainMenu();

/**
 * @brief Exibe uma mensagem de boas-vindas para um utilizador autenticado.
 * @param username O nome de utilizador a ser exibido.
 */
void displayWelcomeMessage(const std::string& username);

/**
 * @brief Ponto de entrada principal da aplicação.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int main() {
    setupConsole();

    // --- Inicialização dos Gestores de Dados ---
    DataManager userDataManager("users.txt");
    DataManager booksDataManager("books.txt");
    DataManager historyDataManager("history.txt");
    DataManager ratingsDataManager("ratings.txt");

    displayMainMenu();

    std::string username;
    std::string password;
    bool isLoggedIn = false;

    // --- Loop de Autenticação de Utilizador ---
    User currentUser(userDataManager);
    while (!isLoggedIn) {
        std::cout << std::endl << YELLOW << "-> Usuário: " << RESET;
        std::getline(std::cin >> std::ws, username);
        currentUser.setUsername(username);

        if (!currentUser.exists()) {
            std::cout << username << ", percebi que você não está cadastrado em nosso sistema. Por favor, crie uma senha." << std::endl;
            std::cout << std::endl << YELLOW << "-> Crie uma senha: " << RESET;
            std::getline(std::cin >> std::ws, password);
            currentUser.hashPassword(password);

            if (!currentUser.save()) {
                std::cout << RED << BOLD << "Ocorreu um erro crítico ao salvar o usuário. O programa será encerrado." << RESET << std::endl;
                return 1;
            }
            std::cout << GREEN << "Usuário cadastrado com sucesso!" << RESET << std::endl;
            isLoggedIn = true;
        } else {
            currentUser.load();
            std::string storedPasswordHash = currentUser.getPasswordHash();

            User validator(userDataManager); // Usado apenas para validar a senha inserida
            bool passwordCorrect = false;
            while (!passwordCorrect) {
                std::cout << YELLOW << "-> Senha: " << RESET;
                std::getline(std::cin >> std::ws, password);
                validator.hashPassword(password);
                if (validator.getPasswordHash() == storedPasswordHash) {
                    passwordCorrect = true;
                    isLoggedIn = true;
                } else {
                    std::cout << RED << "Senha incorreta, tente novamente." << RESET << std::endl;
                }
            }
        }
    }

    displayWelcomeMessage(currentUser.getUsername());

    // --- Manipulador de Comandos ---
    std::string userInput;
    bool isRunning = true;
    while (isRunning) {
        std::cout << std::endl << YELLOW << "> " << RESET;
        // Usa getline para ler a linha inteira, permitindo comandos com argumentos
        std::getline(std::cin >> std::ws, userInput);

        if (userInput.empty()) continue;

        std::stringstream ss(userInput);
        std::string command, args;
        ss >> command;
        std::getline(ss, args);

        // Remove espaços em branco à esquerda dos argumentos
        if (!args.empty() && args[0] == ' ') {
            args.erase(0, 1);
        }

        if (command == "sair" || command == "exit") {
            isRunning = false;
        } else if (command == "info") {
            if (args.empty()) {
                std::cout << RED << "Uso: info <ISBN>" << RESET << std::endl;
                continue;
            }
            Book book(args, booksDataManager);
            if (!book.exists()) {
                std::cout << RED << "O livro com o ISBN '" << book.getIsbn() << "' não foi encontrado." << RESET << std::endl;
            } else {
                book.load();
                std::cout << BOLD << "Detalhes do livro: " << RESET << std::endl;
                std::cout << "    * ISBN: " << book.getIsbn() << std::endl;
                std::cout << "    * Título: " << book.getTitle() << std::endl;
                std::cout << "    * Autor: " << book.getAuthor() << std::endl;
                std::cout << "    * Ano de Publicação: " << book.getYear() << std::endl;
                std::cout << "    * Editora: " << book.getPublisher() << std::endl;
                std::cout << "    * Avaliação: " << book.getRating() << "/5.0" << std::endl;
            }
        } else {
            std::cout << RED << "Comando '" << command << "' desconhecido." << RESET << std::endl;
        }
    }

    std::cout << std::endl << GREEN << "Obrigado por utilizar o BookMatch." << RESET << std::endl;
    return 0;
}

void setupConsole() {
    #ifdef _WIN32
        // Define a página de código do console de saída para UTF-8 no Windows
        SetConsoleOutputCP(CP_UTF8);
        // Define a página de código de entrada para UTF-8 no Windows
        SetConsoleCP(CP_UTF8);
    #endif
}

void displayMainMenu() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    std::cout << CYAN << R"(
    $$$$$$$\   $$$$$$\   $$$$$$\  $$\   $$\
    $$  __$$\ $$  __$$\ $$  __$$\ $$ | $$  |
    $$ |  $$ |$$ /  $$ |$$ /  $$ |$$ |$$  /
    $$$$$$$\ |$$ |  $$ |$$ |  $$ |$$$$$  /
    $$  __$$\ $$ |  $$ |$$ |  $$ |$$  $$<
    $$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |\$$\
    $$$$$$$  | $$$$$$  | $$$$$$  |$$ | \$$\
    \_______/  \______/  \______/ \__|  \__|

      $$\      $$\  $$$$$$\ $$$$$$$$\  $$$$$$\  $$\   $$\
      $$$\    $$$ |$$  __$$\\__$$  __|$$  __$$\ $$ |  $$ |
      $$$$\  $$$$ |$$ /  $$ |  $$ |   $$ /  \__|$$ |  $$ |
      $$\$$\$$ $$ |$$$$$$$$ |  $$ |   $$ |      $$$$$$$$ |
      $$ \$$$  $$ |$$  __$$ |  $$ |   $$ |      $$  __$$ |
      $$ |\$  /$$ |$$ |  $$ |  $$ |   $$ |  $$\ $$ |  $$ |
      $$ | \_/ $$ |$$ |  $$ |  $$ |   \$$$$$$  |$$ |  $$ |
      \__|     \__|\__|  \__|  \__|    \______/ \__|  \__|
      )" << RESET << std::endl << std::endl;

    std::cout << YELLOW << BOLD << "Bem-vindo ao BookMatch!" << RESET << std::endl << std::endl;
    std::cout << GREEN << "Digite seu nome de usuário para começar:" << RESET << std::endl;
}

void displayWelcomeMessage(const std::string& username) {
    std::cout << GREEN << "Bem-vindo, " << BOLD << username << "!" << RESET << std::endl;
}
