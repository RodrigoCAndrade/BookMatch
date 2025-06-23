/**
 * @file: DataManager.cpp
 * @author: Rodrigo Andrade
 * @date: 23 Jun 2025
 * @description: Implementação da classe DataManager.
 * @version: 2.1
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
*/

#include "DataManager.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @brief Construtor que assume o diretório padrão "data".
 * @param filename O nome do arquivo a ser gerenciado (ex: "users.txt").
 */
DataManager::DataManager(const std::string& filename)
    : directoryPath("data"), fileName(filename) {
    ensureDirectoryExists();
    // Garante que o arquivo exista
    std::ofstream f(getFullPath(), std::ios::app);
}

/**
 * @brief Construtor que permite especificar um diretório e nome de arquivo.
 * @param filename O nome do arquivo a ser gerenciado.
 * @param directory O caminho do diretório onde o arquivo está localizado.
 */
DataManager::DataManager(const std::string& filename, const std::string& directory)
    : directoryPath(directory), fileName(filename) {
    ensureDirectoryExists();
    std::ofstream f(getFullPath(), std::ios::app);
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

/**
 * @brief Salva o objeto JSON no arquivo, usando escrita atômica para evitar corrupção de dados.
 * @param j O objeto JSON a ser salvo.
 * @return true se o salvamento foi bem-sucedido, false caso contrário.
 */
bool DataManager::save(json &j) {
    std::string tempPath = getFullPath() + ".tmp";
    {
        std::ofstream tempFile(tempPath, std::ios::trunc);
        if (!tempFile.is_open()) return false;
        tempFile << j.dump(4);
        tempFile.close();
    }
    try {
        std::filesystem::rename(tempPath, getFullPath());
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Erro ao salvar JSON: " << e.what() << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief Carrega o objeto JSON do arquivo. Se uma query é fornecida, retorna apenas o valor correspondente à chave.
 * @param query A chave do valor a ser retornado (opcional).
 * @return O objeto JSON carregado, ou um objeto JSON vazio em caso de erro.
 */
json DataManager::load(std::string query) {
    json data = load();
    if (data.contains(query)) return data[query];
    return json::object();
}

/**
 * @brief Carrega o objeto JSON principal do arquivo.
 * @return O objeto JSON carregado, ou um objeto JSON vazio em caso de erro.
 */
json DataManager::load() {
    std::ifstream file(getFullPath());
    if (!file.is_open()) return json::object();
    file.seekg(0, std::ios::end);
    if (file.tellg() == 0) {
        file.close();
        return json::object();
    }
    file.seekg(0, std::ios::beg);
    json data;
    file >> data;
    file.close();
    return data;
}

/**
 * @brief Remove um elemento do objeto JSON no arquivo, se existir.
 * @param query A chave do elemento a ser removido.
 * @return true se o elemento foi removido com sucesso, false caso contrário.
 */
bool DataManager::remove(std::string query) {
    json data = load();
    if (data.is_null() || data.empty()) return false;
    if (!data.contains(query)) return false;
    data.erase(query);
    return save(data);
}

/**
 * @brief Verifica se um elemento existe no objeto JSON do arquivo.
 * @param query A chave do elemento a ser verificado.
 * @return true se o elemento existe, false caso contrário.
 */
bool DataManager::has(std::string query) {
    json data = load();
    if (data.is_null() || data.empty()) return false;
    return data.contains(query);
}
