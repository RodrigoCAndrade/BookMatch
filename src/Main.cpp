/**
 * @file: Main.cpp
 * @authors:
 * Rodrigo de Carvalho Andrade - Matrícula: 9861
 * Gustavo Prevelato Oliveira - Matrícula: 2137
 * Leonardo da Silva Tabuenca - Matrícula: 390
 * Iasmim Evelin Teodoro de Gois - 1051
 * @date: 23 Jun 2025
 * @description: Classe principal para a aplicação BookMatch.
 * @version: 2.1
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
 */

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
// Para não dar conflito entre std::byte e os headers do Windows
#define byte win_byte_override
#include <tabulate/table.hpp>
#undef byte
#include <set>
#include <vector>

#include "Book/Book.h"
#include "DataManager/DataManager.h"
#include "History/History.h"
#include "User/User.h"
#include "Utils/FormatAux.h"

using json = nlohmann::json;
using namespace std;
using namespace tabulate;

// Bloco para configurar o terminal do Windows para aceitar caracteres UTF-8
#ifdef _WIN32
// Define NOMINMAX para evitar conflitos com min e max
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// --- Constantes para Cores ANSI ---
// Usadas para formatar a saída do console de forma portável.
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";

/**
 * @brief Configura o console para a saída de caracteres UTF-8.
 * @note Esta função é específica para o sistema operacional Windows.
 */
void setupConsole();

/**
 * @brief Exibe o menu principal com a arte ASCII do BookMatch.
 */
void displayMainMenu();

/**
 * @brief Exibe uma mensagem de boas-vindas para um utilizador autenticado.
 * @param username O nome de utilizador a ser exibido.
 */
void displayWelcomeMessage(const string& username);

/**
 * @brief Busca livros pelo título usando distância de Jaro-Winkler.
 * @param query Termo de busca.
 * @param result_limit Número máximo de resultados.
 * @param booksDataManager Gerenciador de dados dos livros.
 */
void search(const string& query, unsigned int result_limit,
            DataManager& booksDataManager);

/**
 * @brief Exibe recomendações de livros para o usuário na home page.
 * @param booksDataManager Gerenciador de dados dos livros.
 * @param historyDataManager Gerenciador de histórico.
 * @param currentUser Usuário atual.
 */
void homePage(DataManager& booksDataManager, DataManager& historyDataManager,
              User& currentUser);

/**
 * @brief Exibe a lista de comandos disponíveis e suas utilizações.
 */
void displayHelp() {
  cout << endl << BOLD << "Comandos disponíveis:" << RESET << endl;
  cout << YELLOW << "* ajuda" << RESET
       << " - Exibe a lista de comandos disponiveis." << endl;
  cout << YELLOW << "* info <ISBN>" << RESET
       << " - Exibe detalhes de um livro pelo ISBN e adiciona ao histórico."
       << endl;
  cout << YELLOW << "* busca <termo>" << RESET << " - Busca livros pelo título."
       << endl;
  cout << YELLOW << "* historico" << RESET
       << " - Exibe o histórico de livros consultados." << endl;
  cout << YELLOW << "* homepage" << RESET << " - Exibe recomendações de livros."
       << endl;
  cout << YELLOW << "* sair" << RESET << " - Encerra o programa." << endl;
}

/**
 * @brief Ponto de entrada principal da aplicação.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int main() {
  setupConsole();

  // --- Inicialização dos Gestores de Dados ---
  DataManager userDataManager("users.json");
  DataManager booksDataManager("books.json");
  DataManager historyDataManager("history.json");
  DataManager ratingsDataManager("ratings.json");

  displayMainMenu();

  string username;
  string password;
  bool isLoggedIn = false;

  // --- Loop de Autenticação de Utilizador ---
  User currentUser(userDataManager);
  while (!isLoggedIn) {
    cout << endl << YELLOW << "-> Usuário: " << RESET;
    getline(cin >> ws, username);
    currentUser.setUsername(username);

    if (!currentUser.exists()) {
      cout << username
           << ", percebi que você não está cadastrado em nosso sistema. "
              "Por favor, crie uma senha."
           << endl;
      cout << endl << YELLOW << "-> Crie uma senha: " << RESET;
      getline(cin >> ws, password);
      currentUser.hashPassword(password);

      if (!currentUser.save()) {
        cout << RED << BOLD
             << "Ocorreu um erro crítico ao salvar o usuário. O programa "
                "será encerrado."
             << RESET << endl;
        return 1;
      }
      cout << GREEN << "Usuário cadastrado com sucesso!" << RESET << endl;
      isLoggedIn = true;
    } else {
      currentUser.load();
      string storedPasswordHash = currentUser.getPasswordHash();

      User validator(
          userDataManager);  // Usado apenas para validar a senha inserida
      bool passwordCorrect = false;
      while (!passwordCorrect) {
        cout << YELLOW << "-> Senha: " << RESET;
        getline(cin >> ws, password);
        validator.hashPassword(password);
        if (validator.getPasswordHash() == storedPasswordHash) {
          passwordCorrect = true;
          isLoggedIn = true;
        } else {
          cout << RED << "Senha incorreta, tente novamente." << RESET << endl;
        }
      }
    }
  }

  // Exibindo mensagem de boas-vindas.
  displayWelcomeMessage(currentUser.getUsername());
  // Exibindo recomendações iniciais
  homePage(booksDataManager, historyDataManager, currentUser);

  // --- Manipulador de Comandos ---
  string userInput;
  bool isRunning = true;
  while (isRunning) {
    cout << endl << YELLOW << "> " << RESET;
    // Usa getline para ler a linha inteira, permitindo comandos com argumentos
    getline(cin >> ws, userInput);

    if (userInput.empty()) continue;

    stringstream ss(userInput);
    string command, args;
    ss >> command;
    getline(ss, args);

    // Remove espaços em branco à esquerda dos argumentos
    if (!args.empty() && args[0] == ' ') {
      args.erase(0, 1);
    }

    if (command == "sair" || command == "exit") {
      isRunning = false;
      continue;
    } else if (command == "ajuda" || command == "help") {
      displayHelp();
      continue;
    } else if (command == "info") {
      if (args.empty()) {
        cout << RED << "Uso: info <ISBN>" << RESET << endl;
        continue;
      }

      Book book(args, booksDataManager);
      string isbn = book.getIsbn();
      if (!book.exists()) {
        cout << RED << "O livro com o ISBN '" << isbn << "' não foi encontrado."
             << RESET << endl;
      } else {
        book.load();
        History history(historyDataManager, currentUser);
        history.add(isbn);

        cout << endl
             << BOLD << "Detalhes do Livro (" << isbn << ")" << RESET << endl;
        book.display();
      }
    } else if (command == "busca" || command == "buscar" ||
               command == "search" || command == "query") {
      search(args, 10, booksDataManager);
    } else if (command == "historico" || command == "history") {
      History history(historyDataManager, currentUser);
      vector<string> userHistory = history.get();
      if (userHistory.empty()) {
        cout << RED << "Seu histórico está vazio." << RESET << endl;
      } else {
        cout << endl
             << BOLD << "Histórico de Livros Consultados:" << RESET << endl;
        for (size_t i = 0; i < userHistory.size(); ++i) {
          Book book(userHistory[i], booksDataManager);
          string displayTitle;
          if (book.exists() && book.load()) {
            displayTitle = book.getTitle();
            if (displayTitle.empty()) displayTitle = "[...]";
          } else {
            displayTitle = "[...]";
          }
          cout << YELLOW << i + 1 << ". " << displayTitle
               << " - ISBN: " << userHistory[i] << RESET << endl;
        }
      }
    } else if (command == "homepage" || command == "casa" ||
               command == "recomendacoes" || command == "recommendations") {
      homePage(booksDataManager, historyDataManager, currentUser);
    } else {
      cout << RED << "Comando '" << command << "' desconhecido." << RESET
           << endl;
    }
  }

  cout << endl
       << GREEN << "Obrigado por utilizar o BookMatch." << RESET << endl;
  return 0;
}

void setupConsole() {
#ifdef _WIN32
  // Define a página de código do console de saída para UTF-8 no Windows
  SetConsoleOutputCP(CP_UTF8);
  // Define a página de código de entrada para UTF-8 no Windows
  SetConsoleCP(CP_UTF8);
#endif
}

void displayMainMenu() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif

  cout << CYAN << R"(
    $$$$$$$\   $$$$$$\   $$$$$$\  $$\   $$\
    $$  __$$\ $$  __$$\ $$  __$$\ $$ | $$  |
    $$ |  $$ |$$ /  $$ |$$ /  $$ |$$ |$$  /
    $$$$$$$\ |$$ |  $$ |$$ |  $$ |$$$$$  /
    $$  __$$\ $$ |  $$ |$$ |  $$ |$$  $$<
    $$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |\$$\
    $$$$$$$  | $$$$$$  | $$$$$$  |$$ | \$$\
    \_______/  \______/  \______/ \__|  \__|

      $$\      $$\  $$$$$$\ $$$$$$$$\  $$$$$$\  $$\   $$\
      $$$\    $$$ |$$  __$$\\__$$  __|$$  __$$\ $$ |  $$ |
      $$$$\  $$$$ |$$ /  $$ |  $$ |   $$ /  \__|$$ |  $$ |
      $$\$$\$$ $$ |$$$$$$$$ |  $$ |   $$ |      $$$$$$$$ |
      $$ \$$$  $$ |$$  __$$ |  $$ |   $$ |      $$  __$$ |
      $$ |\$  /$$ |$$ |  $$ |  $$ |   $$ |  $$\ $$ |  $$ |
      $$ | \_/ $$ |$$ |  $$ |  $$ |   \$$$$$$  |$$ |  $$ |
      \__|     \__|\__|  \__|  \__|    \______/ \__|  \__|
      )"
       << RESET << endl
       << endl;

  cout << YELLOW << BOLD << "Bem-vindo ao BookMatch!" << RESET << endl << endl;
  cout << GREEN << "Digite seu nome de usuário para começar:" << RESET << endl;
}

void displayWelcomeMessage(const string& username) {
  cout << GREEN << "Bem-vindo, " << BOLD << username << "!" << RESET << endl;
}

// Jaro-Winkler Similarity (retorna valor entre 0.0 e 1.0, quanto maior mais
// parecido)
// Referência: https://www.geeksforgeeks.org/jaro-and-jaro-winkler-similarity/
double jaroWinkler(const string& s1, const string& s2) {
  const size_t len1 = s1.size();
  const size_t len2 = s2.size();
  if (len1 == 0 && len2 == 0) return 1.0;
  if (len1 == 0 || len2 == 0) return 0.0;
  const size_t match_distance = max(len1, len2) / 2 - 1;
  vector<bool> s1_matches(len1, false);
  vector<bool> s2_matches(len2, false);
  int matches = 0;
  for (size_t i = 0; i < len1; ++i) {
    size_t start = (i >= match_distance) ? i - match_distance : 0;
    size_t end = min(i + match_distance + 1, len2);
    for (size_t j = start; j < end; ++j) {
      if (s2_matches[j]) continue;
      if (s1[i] != s2[j]) continue;
      s1_matches[i] = true;
      s2_matches[j] = true;
      ++matches;
      break;
    }
  }
  if (matches == 0) return 0.0;
  double t = 0.0;
  int k = 0;
  for (size_t i = 0; i < len1; ++i) {
    if (!s1_matches[i]) continue;
    while (!s2_matches[k]) ++k;
    if (s1[i] != s2[k]) t += 0.5;
    ++k;
  }
  double m = matches;
  double jaro = (m / len1 + m / len2 + (m - t) / m) / 3.0;
  // Winkler boost
  int prefix = 0;
  for (size_t i = 0; i < min({len1, len2, size_t(4)}); ++i) {
    if (s1[i] == s2[i])
      ++prefix;
    else
      break;
  }
  return jaro + 0.1 * prefix * (1.0 - jaro);
}

void search(const string& query, unsigned int result_limit,
            DataManager& booksDataManager) {
  // Carrega todos os livros do JSON
  json books = booksDataManager.load();
  if (books.empty()) {
    cout << "Nenhum livro encontrado." << endl;
    return;
  }

  FormatAux formatAux = FormatAux();

  vector<pair<double, string>> results;  // (similaridade, ISBN)
  // Similaridade Jaro-Winkler
  string queryLower = formatAux.toLower(query);
  string queryNorm = formatAux.removeAccents(queryLower);

  for (auto it = books.begin(); it != books.end(); ++it) {
    string isbn = it.key();
    string title = it.value().value("title", "");

    string titleLower = formatAux.toLower(title);
    string titleNorm = formatAux.removeAccents(titleLower);

    double sim = jaroWinkler(queryNorm, titleNorm);

    // Limite de similaridade
    if (sim <= 0.67) continue;
    results.emplace_back(sim, isbn);
  }

  if (results.empty()) {
    cout << "Nenhum resultado encontrado para '" << query << "'." << endl;
    return;
  }

  sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
    return a.first > b.first;  // Maior similaridade primeiro
  });

  cout << endl
       << BOLD << "Resultados da Busca para '" << query << "'" << RESET << endl;

  Table table;
  table.add_row({"#", "ISBN", "Título", "Autor", "Similaridade"});

  for (size_t i = 0; i < min(results.size(), (size_t)result_limit); ++i) {
    const auto& book = books[results[i].second];
    string title = book.value("title", "");
    string author = book.value("author", "");

    // Trunca strings longas para caber nas colunas
    if (title.length() > 45) title = title.substr(0, 42) + "...";
    if (author.length() > 27) author = author.substr(0, 24) + "...";

    stringstream similarity_ss;
    similarity_ss << fixed << setprecision(2) << results[i].first;

    table.add_row({to_string(i + 1), results[i].second, title, author,
                   similarity_ss.str()});
  }

  // Formatação da tabela
  for (size_t i = 0; i < 5; ++i) {
    table[0][i]
        .format()
        .font_style({FontStyle::bold})
        .font_align(FontAlign::center);
  }
  table.column(0).format().font_align(FontAlign::center);
  table.column(4).format().font_align(FontAlign::center);

  table.format()
      .border_top("─")
      .border_bottom("─")
      .border_left("│")
      .border_right("│")
      .column_separator("│");

  cout << table << endl;
}

void homePage(DataManager& booksDataManager, DataManager& historyDataManager,
              User& currentUser) {
  History history(historyDataManager, currentUser);
  vector<string> userHistory = history.get();
  json books = booksDataManager.load();
  vector<pair<string, string>> recommendations;  // (ISBN, Título)

  set<string> userTags;
  size_t lastN = min(userHistory.size(), size_t(3));
  // Coleta tags dos últimos N livros do histórico (mais recentes)
  for (size_t idx = 0; idx < lastN; ++idx) {
    size_t i = userHistory.size() - 1 - idx;
    Book book(userHistory[i], booksDataManager);
    if (book.exists() && book.load()) {
      vector<string> tags = book.getTags();
      for (const auto& tag : tags) {
        if (!tag.empty()) userTags.insert(tag);
      }
    }
  }

  // Mapeia ISBN para (qtd_tags_em_comum, createdDate)
  vector<tuple<int, string, string>>
      candidates;  // (qtd_tags, createdDate, ISBN)
  for (auto it = books.begin(); it != books.end(); ++it) {
    string isbn = it.key();
    if (find(userHistory.begin(), userHistory.end(), isbn) != userHistory.end())
      continue;
    set<string> bookTags;
    if (it.value().contains("tags")) {
      if (it.value()["tags"].is_array()) {
        for (const auto& tag : it.value()["tags"]) {
          if (tag.is_string()) bookTags.insert(tag.get<string>());
        }
      } else if (it.value()["tags"].is_string()) {
        string tagStr = it.value()["tags"].get<string>();
        if (!tagStr.empty()) bookTags.insert(tagStr);
      }
    }
    int common = 0;
    for (const auto& tag : bookTags) {
      if (!tag.empty() && userTags.count(tag)) ++common;
    }
    if (!userTags.empty() && common > 0) {
      string createdDate = it.value().value("createdDate", "");
      candidates.emplace_back(common, createdDate, isbn);
    }
  }

  // Ordena por qtd_tags_em_comum (desc), depois por createdDate (desc)
  sort(candidates.begin(), candidates.end(), [](const auto& a, const auto& b) {
    if (get<0>(a) != get<0>(b)) return get<0>(a) > get<0>(b);
    return get<1>(a) > get<1>(b);
  });

  for (size_t i = 0; i < min(candidates.size(), size_t(3)); ++i) {
    string isbn = get<2>(candidates[i]);
    string title = books[isbn].value("title", "[...]");
    recommendations.emplace_back(isbn, title);
  }

  // Se não houver recomendações por tags, recomenda os mais recentes
  if (recommendations.empty()) {
    vector<pair<string, string>> bookDates;  // (ISBN, createdDate)
    for (auto it = books.begin(); it != books.end(); ++it) {
      string isbn = it.key();
      if (!userHistory.empty() && find(userHistory.begin(), userHistory.end(),
                                       isbn) != userHistory.end())
        continue;
      string createdDate = it.value().value("createdDate", "");
      bookDates.emplace_back(isbn, createdDate);
    }
    sort(bookDates.begin(), bookDates.end(), [](const auto& a, const auto& b) {
      return a.second > b.second;  // Mais recente primeiro
    });
    for (size_t i = 0; i < min(bookDates.size(), size_t(3)); ++i) {
      string isbn = bookDates[i].first;
      string title = books[isbn].value("title", "[...]");
      recommendations.emplace_back(isbn, title);
    }
  }

  cout << endl << BOLD << "Recomendações para você:" << RESET << endl;
  if (recommendations.empty()) {
    cout << YELLOW << "Nenhuma recomendação disponível no momento." << RESET
         << endl;
  } else {
    for (size_t i = 0; i < recommendations.size(); ++i) {
      cout << GREEN << i + 1 << ". " << recommendations[i].second
           << " - ISBN: " << recommendations[i].first << RESET << endl;
    }
  }
}
