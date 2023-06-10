//
// Created by lowern1ght on 07.06.2023.
//

#ifndef COPY_MOD_SRC_MODULES_LOGGER_H
#define COPY_MOD_SRC_MODULES_LOGGER_H

#include <filesystem>

using namespace std;
using namespace filesystem;

enum logger_type {
  error, info, warning
};

class logger {
private:
  path *path_to_logger_file;
  bool write_to_console = true;
  long long rotation_size;
private:
  void
  check_log_file_size_and_clean();

public:
  logger(path *path_to_logger_file, bool write_to_console, int rotation_size);
public:
  void
  write_message(string message, logger_type type, bool pwrite_to_console = true);
};

string
get_enum_name(logger_type type);

#endif //COPY_MOD_SRC_MODULES_LOGGER_H
