#include "FormatAux.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

FormatAux::FormatAux() {}

string FormatAux::toLower(const string& str) {
  string lower_str;
  lower_str.reserve(str.length());
  for (size_t i = 0; i < str.length(); ++i) {
    unsigned char c1 = str[i];
    if (c1 >= 'A' && c1 <= 'Z') {
      lower_str += c1 + ('a' - 'A');
    } else if (c1 == 0xC3 &&
               i + 1 < str.length()) {  // Caracteres acentuados maiúsculos em
                                        // UTF-8 (2 bytes)
      unsigned char c2 = str[i + 1];
      if (c2 >= 0x80 && c2 <= 0x9C) {  // Á, À, Â, Ã, Ä, ... , Ü
        lower_str += c1;
        lower_str += c2 + 0x20;  // Converte para o correspondente minúsculo
        i++;
      } else {
        lower_str += c1;
      }
    } else {
      lower_str += c1;
    }
  }
  return lower_str;
}

string FormatAux::join(const vector<string>& vec, const string& sep) {
  ostringstream oss;
  for (size_t i = 0; i < vec.size(); ++i) {
    if (i != 0) oss << sep;
    oss << vec[i];
  }
  return oss.str();
}

string FormatAux::removeAccents(const string& str) {
  string result;
  result.reserve(str.length());
  for (size_t i = 0; i < str.length(); ++i) {
    unsigned char c1 = str[i];
    if (c1 == 0xC3 &&
        i + 1 < str.length()) {  // Início de um caractere de 2 bytes em UTF-8
      unsigned char c2 = str[i + 1];
      switch (c2) {
        case 0xA0:
        case 0xA1:
        case 0xA2:
        case 0xA3:
        case 0xA4:
        case 0xA5:  // à,á,â,ã,ä,å
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:  // À,Á,Â,Ã,Ä,Å
          result += 'a';
          break;
        case 0xA8:
        case 0xA9:
        case 0xAA:
        case 0xAB:  // è,é,ê,ë
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:  // È,É,Ê,Ë
          result += 'e';
          break;
        case 0xAC:
        case 0xAD:
        case 0xAE:
        case 0xAF:  // ì,í,î,ï
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:  // Ì,Í,Î,Ï
          result += 'i';
          break;
        case 0xB2:
        case 0xB3:
        case 0xB4:
        case 0xB5:
        case 0xB6:
        case 0xB8:  // ò,ó,ô,õ,ö,ø
        case 0x92:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x98:  // Ò,Ó,Ô,Õ,Ö,Ø
          result += 'o';
          break;
        case 0xB9:
        case 0xBA:
        case 0xBB:
        case 0xBC:  // ù,ú,û,ü
        case 0x99:
        case 0x9A:
        case 0x9B:
        case 0x9C:  // Ù,Ú,Û,Ü
          result += 'u';
          break;
        case 0xA7:
        case 0x87:  // ç, Ç
          result += 'c';
          break;
        case 0xB1:
        case 0x91:  // ñ, Ñ
          result += 'n';
          break;
        default:
          result += c1;
          result += c2;
          break;
      }
      i++;  // Pula o segundo byte do caractere UTF-8
    } else {
      result += c1;
    }
  }
  return result;
}
