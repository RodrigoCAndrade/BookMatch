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
        string data_path;
        string data_filename;
        fstream file;

    public:
        /**
         * @brief Construtor da classe Data
         *
         * Inicializa os caminhos e cria o diretório de dados se não existir
         */
        Data() {
            data_path = "data";
            data_filename = "data.txt";

            // Cria o diretório data se não existir
            system("mkdir data 2>nul");

            // Cria o arquivo data.txt se não existir
            openFile(ios::out | ios::app);
            closeFile();
        }

        /**
         * @brief Construtor da classe Data com nome de arquivo personalizados
         *
         * @param filename Nome do arquivo/diretório
        */
        Data(string filename) : data_filename(filename), data_path("data") {
            // Cria o diretório data se não existir
            system("mkdir data 2>nul");

            // Cria o arquivo se não existir
            openFile(ios::out | ios::app);
            closeFile();
        }

        /**
         * @brief Construtor da classe Data com caminho e nome de arquivo personalizados
         *
         * @param filename Nome do arquivo/diretório
         * @param path Caminho onde será criado o diretório
         */
        Data(string filename, string path) : data_filename(filename), data_path(path) {
            // Cria o diretório especificado se não existir
            string command = "mkdir \"" + path + "/" + filename + "\" 2>nul";
            system(command.c_str());

            // Cria o arquivo se não existir
            openFile(ios::out | ios::app);
            closeFile();
        }

        /**
         * @brief Abre um arquivo para leitura/escrita
         * @param filename Nome do arquivo
         * @param mode Modo de abertura (padrão: leitura e escrita)
         * @return true se o arquivo foi aberto com sucesso
         */
        bool openFile(ios_base::openmode mode = ios::in | ios::out);

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
        string getPath();

        /**
         * @brief Obtém o caminho completo do arquivo
         * @return Caminho completo do arquivo
         */
        string getFilename();
};

#endif
