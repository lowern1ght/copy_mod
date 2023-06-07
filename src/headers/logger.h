//
// Created by lowern1ght on 07.06.2023.
//

#ifndef COPY_MOD_SRC_MODULES_LOGGER_H
#define COPY_MOD_SRC_MODULES_LOGGER_H

#include <filesystem>

using namespace std;
using namespace filesystem;

class logger {
private:
  path* path_to_logger_file;
  bool write_to_console = true;
public:
  logger(path* path_to_logger_file, bool write_to_console);
};

#endif //COPY_MOD_SRC_MODULES_LOGGER_H
