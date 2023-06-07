//
// Created by lowern1ght on 06.06.2023.
//

#ifndef COPY_MOD_SRC_MODULES_CONTROLLER_H
#define COPY_MOD_SRC_MODULES_CONTROLLER_H

#include <bitset>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <openssl/md5.h>

using namespace std;

class controller {
public:
  std::string get_hash_summary(const std::filesystem::path entity);
  std::string get_byte_entity(std::filesystem::path entity);
};

#endif //COPY_MOD_SRC_MODULES_CONTROLLER_H
