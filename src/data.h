/**
 * @file data.h
 * @brief Classe responsável pelo gerenciamento de arquivos e diretórios do sistema
 * @author Rodrigo Andrade
 * @date 11.06.2025
 * @version 1.0
 * @license MIT
 */

#ifndef DATA_H
#define DATA_H

#include <string>
#include <fstream>

using namespace std;

/**
 * @class Data
 * @brief Gerencia operações de armazenamento de dados tanto dos usuários quanto dos livros.
 *
 * Esta classe é responsável por:
 * - Gerenciar o diretório de dados do sistema
 * - Manipular arquivos de usuários e livros
 * - Fornecer métodos para leitura e escrita em arquivos
 */

class Data {
    private:
        string data_path;        ///< Caminho do diretório de dados
        string user_data_filename; ///< Nome do arquivo de usuários
        string book_data_filename; ///< Nome do arquivo de livros
        fstream file;

    public:
        /**
         * @brief Construtor da classe Data
         *
         * Inicializa os caminhos e cria o diretório de dados se não existir
         */
        Data() {
            data_path = "data";
            user_data_filename = "users.txt";
            book_data_filename = "books.txt";

            // Cria o diretório data se não existir
            system("mkdir data 2>nul");
        }

        /**
         * @brief Abre um arquivo para leitura/escrita
         * @param filename Nome do arquivo
         * @param mode Modo de abertura (padrão: leitura e escrita)
         * @return true se o arquivo foi aberto com sucesso
         */
        bool openFile(string filename, ios_base::openmode mode = ios::in | ios::out);

        /**
         * @brief Fecha o arquivo atual
         */
        void closeFile();

        /**
         * @brief Verifica se há um arquivo aberto
         * @return true se houver um arquivo aberto
         */
        bool isFileOpen();

        /**
         * @brief Escreve conteúdo no arquivo
         * @param content Conteúdo a ser escrito
         * @return true se a escrita foi bem-sucedida
         */
        bool writeToFile(string content);

        /**
         * @brief Lê todo o conteúdo do arquivo
         * @return Conteúdo do arquivo
         */
        string readFromFile();

        /**
         * @brief Lê uma linha do arquivo
         * @param line Referência para armazenar a linha lida
         * @return true se uma linha foi lida com sucesso
         */
        bool getLine(string& line);

        /**
         * @brief Obtém o caminho do diretório de dados
         * @return Caminho do diretório
         */
        string getDataPath() { return data_path; }

        /**
         * @brief Obtém o caminho completo do arquivo de usuários
         * @return Caminho completo do arquivo
         */
        string getUserDataFilename() { return data_path + "/" + user_data_filename; }

        /**
         * @brief Obtém o caminho completo do arquivo de livros
         * @return Caminho completo do arquivo
         */
        string getBookDataFilename() { return data_path + "/" + book_data_filename; }
};

#endif
