/**
 * @file: book.cpp
 * @author: Rodrigo Andrade
 * @date: 23 Jun 2025
 * @description: Implementação da classe Book.
 * @version: 2.1
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
 */

#include "Book.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#define byte win_byte_override
#include <tabulate/table.hpp>
#undef byte
#include <vector>

#include "../Utils/FormatAux.h"

using json = nlohmann::json;
using namespace tabulate;

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
void Book::setPublisher(const std::string& publisher) {
  this->publisher = publisher;
}
std::string Book::getGenre() const { return this->genre; }
void Book::setGenre(const std::string& genre) { this->genre = genre; }
std::string Book::getDescription() const { return this->description; }
void Book::setDescription(const std::string& description) {
  this->description = description;
}
float Book::getRating() const { return this->rating; }
void Book::setRating(float rating) { this->rating = rating; }

/**
 * @brief Verifica se um livro com o ISBN atual existe no arquivo.
 * @return true se o livro existe, false caso contrário.
 */
bool Book::exists() { return dataManager.has(this->isbn); }

/**
 * @brief Carrega os dados do livro do arquivo para este objeto.
 * @return true se o livro foi encontrado e carregado, false caso contrário.
 * @note Requer C++20 para std::views::split.
 */
bool Book::load() {
  json data = dataManager.load(this->isbn);
  if (data.is_null() || data.empty()) return false;
  this->title = data.value("title", "");
  this->author = data.value("author", "");
  this->publisher = data.value("publisher", "");
  this->description = data.value("description", "");
  this->genre = data.value("genre", "");
  this->year = 0;
  std::string date = data.value("date", "");
  if (!date.empty()) {
    for (char c : date) {
      if (isdigit(c)) {
        this->year = std::stoi(date.substr(date.find(c)));
        break;
      }
    }
  }
  this->createdDate = data.value("createdDate", "");
  // Carrega as tags
  tags.clear();
  if (data.contains("tags")) {
    if (data["tags"].is_array()) {
      for (const auto& tag : data["tags"]) {
        if (tag.is_string()) tags.push_back(tag.get<std::string>());
      }
    } else if (data["tags"].is_string()) {
      std::string tagStr = data["tags"].get<std::string>();
      if (!tagStr.empty()) tags.push_back(tagStr);
    }
  }
  return true;
}

/**
 * @brief Salva os dados do livro no arquivo de forma segura.
 * @return true se a operação foi bem-sucedida, false caso contrário.
 */
bool Book::save() {
  json allBooks = dataManager.load("");
  // Salva as tags como array
  allBooks[this->isbn] = {{"title", this->title},
                          {"author", this->author},
                          {"date", std::to_string(this->year)},
                          {"createdDate", this->createdDate},
                          {"publisher", this->publisher},
                          {"description", this->description},
                          {"genre", this->genre},
                          {"tags", this->tags}};
  return dataManager.save(allBooks);
}

/**
 * @brief Remove o livro atual utilizando o ISBN.
 * @return true se a remoção foi bem-sucedida, false caso contrário.
 */
bool Book::remove() { return dataManager.remove(this->isbn); }

bool Book::setTags(vector<string> tags) {
  this->tags = std::move(tags);
  return true;
}

vector<string> Book::getTags() { return this->tags; }

bool Book::addTag(string& tag) {
  // Evita duplicatas
  if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
    tags.push_back(tag);
    return true;
  }
  return false;
}

bool Book::removeTag(string& tag) {
  auto it = std::find(tags.begin(), tags.end(), tag);
  if (it != tags.end()) {
    tags.erase(it);
    return true;
  }
  return false;
}

bool Book::display() {
  FormatAux formatAux = FormatAux();

  Table details;
  details.add_row({"Campo", "Valor"});
  details.add_row({"ISBN", this->getIsbn()});
  details.add_row({"Título", this->getTitle()});
  details.add_row({"Autor", this->getAuthor()});
  details.add_row({"Ano de Publicação", to_string(this->getYear())});
  details.add_row({"Editora", this->getPublisher()});
  details.add_row({"Tags", formatAux.join(this->getTags())});
  stringstream rating;
  rating << fixed << setprecision(1) << this->getRating() << "/5.0";
  details.add_row({"Avaliação", rating.str()});

  // Formatação
  details.column(0)
      .format()
      .font_style({FontStyle::bold})
      .font_color(Color::yellow);
  details[0][0].format().font_align(FontAlign::center);
  details[0][1].format().font_align(FontAlign::center);

  details.format()
      .border_top("─")
      .border_bottom("─")
      .border_left("│")
      .border_right("│")
      .column_separator("│");

  cout << details << endl;
  return true;
}
