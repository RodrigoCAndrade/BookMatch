#include "History.h"

#include <algorithm>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;
using namespace std;

History::History(DataManager& dataManager, User& user)
    : dataManager(dataManager), user(user) {
  load();
}

/**
 * @brief Carrega o histórico do usuário atual a partir do arquivo JSON.
 */
bool History::load() {
  json allHistoryData = this->dataManager.load();
  string username = this->user.getUsername();

  if (allHistoryData.contains(username)) {
    this->history = allHistoryData[username].get<vector<string>>();
  } else
    this->history.clear();

  return true;
}

/**
 * @brief Salva o histórico do usuário atual de volta no arquivo JSON.
 */
bool History::save() {
  json allHistoryData = this->dataManager.load();
  allHistoryData[this->user.getUsername()] = this->history;
  return this->dataManager.save(allHistoryData);
}

/**
 * @brief Retorna uma referência ao objeto do usuário.
 * Retornar uma referência é mais eficiente, pois evita fazer uma cópia.
 */
User& History::getUser() { return this->user; }

/**
 * @brief Retorna uma cópia do vetor de histórico.
 */
vector<string> History::get() { return this->history; }

/**
 * @brief Substitui o histórico atual por um novo.
 */
bool History::set(vector<string>& newHistory) {
  this->history = newHistory;
  return true;
}

/**
 * @brief Adiciona o ISBN de um livro ao histórico.
 * Para evitar duplicatas, primeiro verifica se o ISBN já está presente.
 */
bool History::add(string& isbn) {
  if (find(this->history.begin(), this->history.end(), isbn) ==
      this->history.end()) {
    this->history.push_back(isbn);
  }
  return true;
}

/**
 * @brief Remove o ISBN de um livro do histórico.
 * A função original não recebia nenhum argumento, então eu a alterei para
 * aceitar o ISBN do livro a ser removido.
 */
bool History::remove(string &isbn) {
  auto it = std::remove(this->history.begin(), this->history.end(), isbn);
  if (it != this->history.end()) {
    this->history.erase(it, this->history.end());
    return true;
  }
  return false;
}

/**
 * @brief Limpa todos os itens do histórico do usuário atual.
 */
bool History::clear() {
  this->history.clear();
  return true;
}
