/**
 * @file: book.cpp
 * @author: Rodrigo Andrade
 * @date: 16 Jun 2025
 * @description: Implementação da classe Book.
 * @version: 2.0
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
*/

#include "book.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <ranges>
#include <string_view>

/**
 * @brief Construtor da classe Book.
 * @param isbn O ISBN do livro.
 * @param dataManager Uma referência ao gerenciador de dados.
 */
Book::Book(const std::string& isbn, DataManager& dataManager)
    : isbn(isbn), dataManager(dataManager) {}

// --- Getters & Setters ---

std::string Book::getIsbn() const { return this->isbn; }
void Book::setIsbn(const std::string& isbn) { this->isbn = isbn; }
std::string Book::getTitle() const { return this->title; }
void Book::setTitle(const std::string& title) { this->title = title; }
std::string Book::getAuthor() const { return this->author; }
void Book::setAuthor(const std::string& author) { this->author = author; }
int Book::getYear() const { return this->year; }
void Book::setYear(int year) { this->year = year; }
std::string Book::getPublisher() const { return this->publisher; }
void Book::setPublisher(const std::string& publisher) { this->publisher = publisher; }
std::string Book::getGenre() const { return this->genre; }
void Book::setGenre(const std::string& genre) { this->genre = genre; }
std::string Book::getDescription() const { return this->description; }
void Book::setDescription(const std::string& description) { this->description = description; }
float Book::getRating() const { return this->rating; }
void Book::setRating(float rating) { this->rating = rating; }

/**
 * @brief Verifica se um livro com o ISBN atual existe no arquivo.
 * @return true se o livro existe, false caso contrário.
 */
bool Book::exists() {
    if (!dataManager.open(std::ios::in)) {
        return false;
    }

    const std::string delimiter = "#;#";
    std::string line;
    bool found = false;
    while (dataManager.getLine(line)) {
        size_t pos = line.find(delimiter);
        if (pos != std::string::npos && line.substr(0, pos) == this->isbn) {
            found = true;
            break;
        }
    }
    dataManager.close();
    return found;
}

/**
 * @brief Carrega os dados do livro do arquivo para este objeto.
 * @return true se o livro foi encontrado e carregado, false caso contrário.
 * @note Requer C++20 para std::views::split.
 */
bool Book::load() {
    if (!dataManager.open(std::ios::in)) {
        return false;
    }

    std::string line;
    const std::string_view delimiter = "#;#";
    bool found = false;

    while (dataManager.getLine(line)) {
        // Verifica se a linha pertence ao livro antes de processá-la
        if (line.rfind(this->isbn, 0) == 0) {
            std::vector<std::string> parts;
            for (const auto& part_view : line | std::views::split(delimiter)) {
                parts.emplace_back(part_view.begin(), part_view.end());
            }

            if (parts.size() == 8) {
                this->title = parts[1];
                this->author = parts[2];
                this->year = std::stoi(parts[3]);
                this->publisher = parts[4];
                this->genre = parts[5];
                this->description = parts[6];
                this->rating = std::stof(parts[7]);
                found = true;
                break;
            }
        }
    }

    dataManager.close();
    return found;
}

/**
 * @brief Salva os dados do livro no arquivo de forma segura.
 * @return true se a operação foi bem-sucedida, false caso contrário.
 */
bool Book::save() {
    std::string originalPath = dataManager.getFullPath();
    std::string tempPath = originalPath + ".tmp";
    const std::string delimiter = "#;#";

    std::ofstream tempFile(tempPath);
    if (!tempFile.is_open()) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo temporario." << std::endl;
        return false;
    }

    std::ifstream originalFile(originalPath);
    if (originalFile.is_open()) {
        std::string line;
        while (std::getline(originalFile, line)) {
            size_t pos = line.find(delimiter);
            if (pos == std::string::npos || line.substr(0, pos) != this->isbn) {
                tempFile << line << std::endl;
            }
        }
    }
    originalFile.close();

    tempFile << this->isbn << delimiter
             << this->title << delimiter
             << this->author << delimiter
             << this->year << delimiter
             << this->publisher << delimiter
             << this->genre << delimiter
             << this->description << delimiter
             << this->rating << std::endl;
    tempFile.close();

    try {
        std::filesystem::rename(tempPath, originalPath);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Erro de Filesystem ao renomear: " << e.what() << std::endl;
        return false;
    }

    return true;
}
