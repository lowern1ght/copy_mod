//
// Created by lowern1ght on 06.06.2023.
//

#ifndef COPY_MOD_SRC_MODULES_COPY_CONFIG_H
#define COPY_MOD_SRC_MODULES_COPY_CONFIG_H

#include <logger.h>
#include <filesystem>

using namespace std::filesystem;

class copy_config {
public:
  bool *check_hash;
  path *to_copy;
  path *from_entity_copy;
  logger *logger;
};

#endif //COPY_MOD_SRC_MODULES_COPY_CONFIG_H
