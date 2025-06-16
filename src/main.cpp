/**
 * @file main.cpp
 * @brief Arquivo principal do sistema BookMatch
 * @author Rodrigo Andrade
 * @date 11.06.2025
 * @version 1.0
 * @license MIT
 */

#include <iostream>
#include <string>
#include <windows.h>
//#include "Book/book.h"
#include "User/user.h"
#include "Data/data.h"

using namespace std;

/**
 * @brief Define a cor do texto no console
 * @param color Código da cor a ser definida
 */
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/**
 * @brief Exibe o menu principal do sistema
 *
 * Mostra o logo ASCII do BookMatch e solicita o nome de usuário
 */
void displayMenu() {
    system("cls"); // Limpa a tela

    // ASCII Art do BookMatch
    setColor(11); // Ciano claro
    cout << R"(
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
      )" << endl << endl;

    setColor(14); // Amarelo
    cout << "Bem-vindo ao BookMatch!" << endl << endl;

    setColor(10); // Verde
    cout << "Digite seu nome de usuário:" << endl;
    setColor(7); // Reset para cor padrão do terminal
}

/**
 * @brief Exibe mensagem de boas-vindas ao usuário
 * @param username Nome do usuário logado
 */
void logar(string username) {
    cout << "Bem-vindo, " << username << "!";
}

/**
 * @brief Função principal do programa
 * @return 0 se executado com sucesso
 */
int main() {
    // Gerenciador de dados
    Data usersData = Data("users.txt");
    Data booksData = Data("books.txt");
    Data historyData = Data("history.txt");
    Data ratingsData = Data("ratings.txt");

    string username;
    string password;

    while (true) {
        displayMenu();
        cout << endl << "-> Usuário: ";
        cin >> username;

        User user(username, usersData);

        int hasUser = user.hasUser();
        // Se o usuário não for cadastrado, inicia o cadastro
        if (hasUser == 0) {
            cout << username << ", percebi que você não está cadastrado em nosso sistema. Por favor, crie uma senha:" << endl;
            cout << endl << "-> Crie uma senha: ";

            cin >> password;

            user.hashPassword(password);
            user.saveUser();
        } else if (hasUser == 1) {
            cout << "-> Senha: ";
            cin >> password;
            // TODO: Implementar verificação de senha
        } else {
            cout << "Ops! Ocorreu um erro inesperado. Por favor, contate um administrador." << endl;
        }
    }

    logar(username);
    return 0;
}
