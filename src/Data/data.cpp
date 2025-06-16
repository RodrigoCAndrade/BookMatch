/**
 * @file data.cpp
 * @brief Implementação da classe Data para gerenciamento de arquivos
 * @author Rodrigo Andrade
 * @date 11.06.2025
 * @version 1.0
 * @license MIT
 */

#include "data.h"

/**
 * @brief Abre um arquivo para leitura/escrita
 * @param mode Modo de abertura do arquivo
 * @return true se o arquivo foi aberto com sucesso
 */
bool Data::openFile(ios_base::openmode mode) {
    if (isFileOpen()) {
        closeFile();
    }

    // Garante que o diretório existe
    string command = "mkdir \"" + getPath() + "\" 2>nul";
    system(command.c_str());

    // Abre o arquivo no caminho completo
    file.open(getPath() + "/" + getFilename(), mode);
    return isFileOpen();
}

/**
 * @brief Fecha o arquivo atualmente aberto
 */
void Data::closeFile() {
    if (isFileOpen()) {
        file.close();
    }
}

/**
 * @brief Verifica se há um arquivo aberto
 * @return true se houver um arquivo aberto
 */
bool Data::isFileOpen() {
    return file.is_open();
}

/**
 * @brief Escreve conteúdo no arquivo
 * @param content Conteúdo a ser escrito
 * @return true se a escrita foi bem-sucedida
 */
bool Data::writeToFile(string content) {
    if (!isFileOpen()) return false;
    file << content;
    return true;
}

/**
 * @brief Lê todo o conteúdo do arquivo
 * @return Conteúdo completo do arquivo
 */
string Data::readFromFile() {
    if (!isFileOpen()) return "";

    string content;
    string line;
    while (getLine(line)) {
        content += line + "\n";
    }
    return content;
}

/**
 * @brief Lê uma linha do arquivo
 * @param line Referência para armazenar a linha lida
 * @return true se uma linha foi lida com sucesso
 */
bool Data::getLine(string& line) {
    if (!isFileOpen()) return false;
    return static_cast<bool>(getline(file, line));
}

/**
 * @brief Obtém o caminho do diretório de dados
 * @return Caminho do diretório
 */
string Data::getPath() {
    return data_path;
}

/**
 * @brief Obtém o caminho completo do arquivo
 * @return Caminho completo do arquivo
 */
string Data::getFilename() {
    return data_filename;
}
