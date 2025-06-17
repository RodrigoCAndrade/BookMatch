/**
 * @file: data_manager.h
 * @author: Rodrigo Andrade
 * @date: 16 Jun 2025
 * @description: Definição da classe DataManager para gerenciar o armazenamento do programa.
 * @version: 2.0
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
*/

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <string>
#include <fstream>
#include <filesystem> // C++17+ para manipulação de sistema de arquivos

/**
 * @class DataManager
 * @brief Gerencia operações de I/O em arquivos de dados.
 *
 * Esta classe abstrai a manipulação de arquivos, incluindo a criação de
 * diretórios e a leitura/escrita de dados, garantindo que os caminhos
 * sejam geridos de forma consistente.
 */
class DataManager {
private:
    std::string directoryPath;
    std::string fileName;
    std::fstream fileStream;

    /**
     * @brief Garante que o diretório de dados exista, criando-o se necessário.
     * @note Utiliza a biblioteca <filesystem> para portabilidade.
     */
    void ensureDirectoryExists();

public:
    /**
     * @brief Construtor que assume o diretório padrão "data".
     * @param filename O nome do arquivo a ser gerenciado (ex: "users.txt").
     */
    explicit DataManager(const std::string& filename);

    /**
     * @brief Construtor que permite especificar um diretório e nome de arquivo.
     * @param filename O nome do arquivo a ser gerenciado.
     * @param directory O caminho do diretório onde o arquivo está localizado.
     */
    DataManager(const std::string& filename, const std::string& directory);

    /**
     * @brief Abre o arquivo associado no modo especificado.
     * @param mode O modo de abertura do arquivo (ex: std::ios::in).
     * @return true se o arquivo foi aberto com sucesso, false caso contrário.
     */
    bool open(std::ios_base::openmode mode);

    /**
     * @brief Fecha o arquivo se estiver aberto.
     */
    void close();

    /**
     * @brief Verifica se o arquivo está atualmente aberto.
     * @return true se o arquivo está aberto, false caso contrário.
     */
    bool isOpen() const;

    /**
     * @brief Lê a próxima linha do arquivo.
     * @param line Uma referência de string para armazenar a linha lida.
     * @return true se uma linha foi lida com sucesso, false caso contrário (ex: fim do arquivo).
     */
    bool getLine(std::string& line);

    // --- Getters ---
    std::string getFileName() const;
    std::string getDirectoryPath() const;
    std::string getFullPath() const;
};

#endif // DATA_MANAGER_H
