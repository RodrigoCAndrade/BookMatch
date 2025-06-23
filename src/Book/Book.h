/**
 * @file: book.h
 * @author: Rodrigo Andrade
 * @date: 16 Jun 2025
 * @description: Definição da classe Book para gerenciar dados dos livros.
 * @version: 2.0
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
*/

#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>
#include "../DataManager/DataManager.h"

using namespace std;

/**
 * @class Book
 * @brief Representa um livro e gerencia suas informações e persistência.
 */
class Book {
private:
    std::string isbn;
    std::string title;
    std::string author;
    int year;
    std::string publisher;
    std::string genre;
    std::string description;
    vector<string> tags;
    float rating;
    DataManager &dataManager;
    std::string createdDate;

public:
    /**
     * @brief Construtor para um objeto Book.
     * @param isbn O ISBN do livro, usado como identificador único.
     * @param dataManager Uma referência ao gerenciador de dados para persistência.
     */
    Book(const std::string& isbn, DataManager& dataManager);

    /**
     * @brief Salva os dados do livro no arquivo.
     * @return true se a operação foi bem-sucedida, false caso contrário.
     */
    bool save();

    /**
     * @brief Carrega os dados do livro do arquivo para este objeto.
     * @return true se o livro foi encontrado e carregado, false caso contrário.
     */
    bool load();

    /**
     * @brief Verifica se um livro com o ISBN atual existe no arquivo.
     * @return true se o livro existe, false caso contrário.
     */
    bool exists();

    /**
     * @brief Remove o livro com o ISBN atual.
     * @return true se removido, false caso contrário.
     */
    bool remove();

    // --- Getters & Setters ---
    std::string getIsbn() const;
    void setIsbn(const std::string& isbn);
    std::string getTitle() const;
    void setTitle(const std::string& title);
    std::string getAuthor() const;
    void setAuthor(const std::string& author);
    int getYear() const;
    void setYear(int year);
    std::string getPublisher() const;
    void setPublisher(const std::string& publisher);
    std::string getGenre() const;
    void setGenre(const std::string& genre);
    std::string getDescription() const;
    void setDescription(const std::string& description);
    float getRating() const;
    void setRating(float rating);
    bool setTags(vector<string> tags);
    vector<string> getTags();
    bool addTag(string &tag);
    bool removeTag(string &tag);
    string getCreatedDate();
    bool setCreatedDate();
    bool display();
};

#endif // BOOK_H
