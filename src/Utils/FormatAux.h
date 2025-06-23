#ifndef FORMAT_AUX_H
#define FORMAT_AUX_H

#include <string>
#include <vector>

using namespace std;

class FormatAux {
 private:
 public:
  FormatAux();
  /**
   * @brief Remove acentos de uma string, tratando caracteres UTF-8 (pt-br).
   * @param str A string de entrada (idealmente já em minúsculas).
   * @return A string sem acentos.
   */
  string removeAccents(const string& str);

  string join(const vector<string>& vec, const string& sep = ", ");

  /**
   * @brief Converte uma string para minúsculas, tratando caracteres UTF-8
   * (pt-br).
   * @param str A string de entrada.
   * @return A string convertida para minúsculas.
   */
  string toLower(const string& str);
};

#endif
