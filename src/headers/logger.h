//
// Created by lowern1ght on 07.06.2023.
//

#ifndef COPY_MOD_SRC_MODULES_LOGGER_H
#define COPY_MOD_SRC_MODULES_LOGGER_H

#include <filesystem>

using namespace std::filesystem;

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
  logger(path *path_to_logger_file, bool write_to_console, long long rotation_size);
public:
  static void log_to_console(const std::string& str, logger_type type);
  void write_message(std::string message, logger_type type, bool pwrite_to_console = true);
};

std::string
get_enum_name(logger_type type);

#endif //COPY_MOD_SRC_MODULES_LOGGER_H
