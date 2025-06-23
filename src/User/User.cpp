/**
 * @file: User.cpp
 * @author: Rodrigo Andrade
 * @date: 23 Jun 2025
 * @description: Implementação da classe User.
 * @version: 2.1
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
 */

#include "User.h"

#include <botan/hash.h>
#include <botan/hex.h>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include <vector>

using json = nlohmann::json;

/**
 * @brief Construtor principal da classe User.
 * @param dataManager Uma referência ao gerenciador de dados.
 */
User::User(DataManager& dataManager) : dataManager(dataManager) {}

// --- Getters & Setters ---

std::string User::getUsername() const { return this->username; }
void User::setUsername(const std::string& username) {
  this->username = username;
}
std::string User::getPasswordHash() const { return this->passwordHash; }
void User::setPasswordHash(const std::string& hash) {
  this->passwordHash = hash;
}

/**
 * @brief Gera o hash de uma senha em plain text usando SHA-512 com a
 * biblioteca Botan.
 * @param plainPassword A senha a ser processada.
 */
void User::hashPassword(const std::string& plainPassword) {
  try {
    std::unique_ptr<Botan::HashFunction> hash_func(
        Botan::HashFunction::create("SHA-512"));
    if (!hash_func) {
      throw std::runtime_error(
          "Não foi possível criar a função de hash SHA-512.");
    }

    hash_func->update(plainPassword);
    this->passwordHash = Botan::hex_encode(hash_func->final());

  } catch (const std::exception& e) {
    throw std::runtime_error("Falha ao gerar o hash da senha com Botan: " +
                             std::string(e.what()));
  }
}

/**
 * @brief Verifica se um usuário com o 'username' atual existe no arquivo.
 * @return true se o usuário existe, false caso contrário.
 */
bool User::exists() { return dataManager.has(getUsername()); }

/**
 * @brief Carrega os dados do usuário do arquivo para este objeto.
 * @return true se o usuário foi encontrado e carregado, false caso contrário.
 */
bool User::load() {
  json userJson = dataManager.load(getUsername());
  setPasswordHash(userJson.value("password", ""));
  return true;
}

/**
 * @brief Salva os dados do usuário (username e hash da senha) no arquivo.
 * @return true se a operação foi bem-sucedida, false caso contrário.
 */
bool User::save() {
  json userJson;
  userJson[getUsername()] = {{"password", getPasswordHash()}};

  return dataManager.save(userJson);
}
