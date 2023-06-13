//
// Created by lowern1ght on 13.06.2023.
//

#ifndef COPY_MOD_SRC_INCLUDES_COLORS_COLORED_H
#define COPY_MOD_SRC_INCLUDES_COLORS_COLORED_H

#include <xstring>
#include <colors/color.hpp>

class colr {
public:
  static std::basic_string<char> green(std::string str);
  static std::basic_string<char> yellow(const std::string& str);
  static std::basic_string<char> red(const std::string& str);
};

#endif //COPY_MOD_SRC_INCLUDES_COLORS_COLORED_H
