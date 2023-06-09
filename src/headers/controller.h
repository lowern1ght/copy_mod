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

using namespace std::filesystem;

class controller {
public:
  std::string static
  get_hash_summary(const path &entity);

  std::string static
  get_byte_entity(const path &entity);
};

#endif //COPY_MOD_SRC_MODULES_CONTROLLER_H
