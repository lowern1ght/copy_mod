//
// Created by lowern1ght on 13.06.2023.
//

#include "colored.h"

std::basic_string<char> colr::green(const std::string& str) {
  return std::string(dye::green(str));
}
std::basic_string<char> colr::yellow(const std::string& str) {
  return std::basic_string<char>();
}
std::basic_string<char> colr::red(const std::string& str) {
  return std::basic_string<char>();
}
