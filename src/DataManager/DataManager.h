/**
 * @file: DataManager.h
 * @author: Rodrigo Andrade
 * @date: 23 Jun 2025
 * @description: Definição da classe DataManager para gerenciar o armazenamento
 * do programa.
 * @version: 2.1
 * @license: MIT
 * @language: C++
 * @github: https://github.com/RodrigoCAndrade/BookMatch
 */

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <filesystem>  // C++17+ para manipulação de sistema de arquivos
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

/**
 * @class DataManager
 * @brief Gerencia operações de I/O em arquivos de dados.
 */
class DataManager {
 private:
  std::string directoryPath;
  std::string fileName;

  /**
   * @brief Garante que o diretório de dados exista, criando-o se necessário.
   * @note Utiliza a biblioteca <filesystem> para portabilidade.
   */
  void ensureDirectoryExists();

 public:
  /**
   * @brief Construtor que assume o diretório padrão "data".
   * @param filename O nome do arquivo a ser gerenciado (ex: "users.txt").
   */
  explicit DataManager(const std::string& filename);

  /**
   * @brief Construtor que permite especificar um diretório e nome de arquivo.
   * @param filename O nome do arquivo a ser gerenciado.
   * @param directory O caminho do diretório onde o arquivo está localizado.
   */
  DataManager(const std::string& filename, const std::string& directory);

  /**
   * @brief Abre o arquivo associado no modo especificado.
   * @param mode O modo de abertura do arquivo (ex: std::ios::in).
   * @return true se o arquivo foi aberto com sucesso, false caso contrário.
   */
  bool open(std::ios_base::openmode mode);

  /**
   * @brief Fecha o arquivo se estiver aberto.
   */
  void close();

  /**
   * @brief Verifica se o arquivo está atualmente aberto.
   * @return true se o arquivo está aberto, false caso contrário.
   */
  bool isOpen() const;

  // --- Getters ---
  std::string getFileName() const;
  std::string getDirectoryPath() const;
  std::string getFullPath() const;

  json getJSON();

  /**
   * @brief Salva um json no arquivo usando atomic write.
   * @return true se o salvamento ocorreu com sucesso, false caso contrário.
   */
  bool save(json& json);

  /**
   * @brief Carrega um json especifico.
   * @return json.
   */
  json load(std::string query);

  /**
   * @brief Carrega todo json.
   * @return json.
   */
  json load();

  /**
   * @brief Remove um json com o termo especifico (Ex:. json que contem certo
   * isbn).
   * @return true se a remoção ocorreu com sucesso, false caso contrário.
   */
  bool remove(std::string query);

  /**
   * @brief Remove um json com o termo especifico (Ex:. json que contem certo
   * isbn).
   * @return true se a remoção ocorreu com sucesso, false caso contrário.
   */
  bool has(std::string query);
};

#endif  // DATA_MANAGER_H
