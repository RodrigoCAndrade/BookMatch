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
 * @param filename Nome do arquivo a ser aberto
 * @param mode Modo de abertura do arquivo
 * @return true se o arquivo foi aberto com sucesso
 */
bool Data::openFile(string filename, ios_base::openmode mode) {
    if (file.is_open()) {
        file.close();
    }
    file.open(filename, mode);
    return file.is_open();
}

/**
 * @brief Fecha o arquivo atualmente aberto
 */
void Data::closeFile() {
    if (file.is_open()) {
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
    if (!file.is_open()) return false;
    file << content;
    return true;
}

/**
 * @brief Lê todo o conteúdo do arquivo
 * @return Conteúdo completo do arquivo
 */
string Data::readFromFile() {
    if (!file.is_open()) return "";

    string content;
    string line;
    while (getline(file, line)) {
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
    if (!file.is_open()) return false;
    return static_cast<bool>(getline(file, line));
}
