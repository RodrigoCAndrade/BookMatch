/**
 * @file: data_manager.cpp
 * @author: Rodrigo Andrade
 * @date: 16 Jun 2025
 * @description: Implementação da classe DataManager.
 * @version: 2.0
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
*/

#include "data_manager.h"
#include <iostream>

/**
 * @brief Construtor que assume o diretório padrão "data".
 * @param filename O nome do arquivo a ser gerenciado (ex: "users.txt").
 */
DataManager::DataManager(const std::string& filename)
    : directoryPath("data"), fileName(filename) {
    ensureDirectoryExists();
    // Garante que o arquivo exista ao abri-lo e fechá-lo em modo append.
    // O modo Append cria o arquivo se ele não existir, sem apagar o conteúdo.
    if (open(std::ios::app)) {
        close();
    }
}

/**
 * @brief Construtor que permite especificar um diretório e nome de arquivo.
 * @param filename O nome do arquivo a ser gerenciado.
 * @param directory O caminho do diretório onde o arquivo está localizado.
 */
DataManager::DataManager(const std::string& filename, const std::string& directory)
    : directoryPath(directory), fileName(filename) {
    ensureDirectoryExists();
    if (open(std::ios::app)) {
        close();
    }
}

/**
 * @brief Garante que o diretório de dados exista, criando-o se necessário.
 */
void DataManager::ensureDirectoryExists() {
    try {
        if (!std::filesystem::exists(this->directoryPath)) {
            std::filesystem::create_directory(this->directoryPath);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Erro de Filesystem ao criar diretório: " << e.what() << std::endl;
    }
}

/**
 * @brief Abre o arquivo associado no modo especificado.
 * @param mode O modo de abertura do arquivo.
 * @return true se o arquivo foi aberto com sucesso, false caso contrário.
 */
bool DataManager::open(std::ios_base::openmode mode) {
    if (isOpen()) {
        close();
    }
    fileStream.open(getFullPath(), mode);
    return isOpen();
}

/**
 * @brief Fecha o arquivo se estiver aberto.
 */
void DataManager::close() {
    if (isOpen()) {
        fileStream.close();
    }
}

/**
 * @brief Verifica se o arquivo está atualmente aberto.
 * @return true se o arquivo está aberto, false caso contrário.
 */
bool DataManager::isOpen() const {
    return fileStream.is_open();
}

/**
 * @brief Lê a próxima linha do arquivo.
 * @param line Uma referência de string para armazenar a linha lida.
 * @return true se uma linha foi lida com sucesso.
 */
bool DataManager::getLine(std::string& line) {
    if (!isOpen()) return false;
    return static_cast<bool>(std::getline(fileStream, line));
}

// --- Getters ---

/**
 * @brief Obtém o nome do arquivo.
 * @return O nome do arquivo.
 */
std::string DataManager::getFileName() const {
    return this->fileName;
}

/**
 * @brief Obtém o caminho do diretório.
 * @return O caminho do diretório.
 */
std::string DataManager::getDirectoryPath() const {
    return this->directoryPath;
}

/**
 * @brief Obtém o caminho completo para o arquivo.
 * @return Uma string contendo "diretorio/nome_do_arquivo".
 */
std::string DataManager::getFullPath() const {
    return this->directoryPath + "/" + this->fileName;
}
