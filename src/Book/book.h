/**
 * @file book.h
 * @brief Definição da classe Book para gerenciamento de livros
 * @author Rodrigo Andrade
 * @date 11.06.2025
 * @version 1.0
 * @license MIT
 */

#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

/**
 * @class Book
 * @brief Classe que representa um livro no sistema
 *
 * Esta classe gerencia todas as informações relacionadas a um livro,
 * incluindo título, autor, ISBN, ano de publicação, editora, gênero,
 * descrição e avaliação.
 */
class Book {
private:
    string title;        // Título do livro
    string author;       // Autor do livro
    string isbn;         // ISBN do livro
    int year;           // Ano de publicação
    string publisher;    // Editora
    string genre;        // Gênero literário
    string description;  // Descrição do livro
    float rating;        // Avaliação do livro (0-5)

public:
    /**
     * @brief Construtor da classe Book
     * @param title Título do livro
     * @param author Autor do livro
     * @param isbn ISBN do livro
     * @param year Ano de publicação
     * @param publisher Editora
     * @param genre Gênero literário
     * @param description Descrição do livro
     * @param rating Avaliação do livro (0-5)
     */
    Book(string title, string author, string isbn, int year, string publisher, string genre, string description, float rating);

    // Getters
    string getTitle();
    string getAuthor();
    string getIsbn();
    int getYear();
    string getPublisher();
    string getGenre();
    string getDescription();
    float getRating();

    // Setters
    void setTitle(string title);
    void setAuthor(string author);
    void setIsbn(string isbn);
    void setYear(int year);
    void setPublisher(string publisher);
    void setGenre(string genre);
    void setDescription(string description);
    void setRating(float rating);

    // Data management
    int saveBook();
    int loadBook(string isbn);
    int hasBook(string isbn);
};

#endif
