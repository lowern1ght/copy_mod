//
// Created by lowern1ght on 07.06.2023.
//

#include <logger.h>

using namespace std;
using namespace filesystem;

logger::logger(path *path_to_logger_file, bool write_to_console = true) {
  this->write_to_console = write_to_console;
  if (path_to_logger_file != nullptr) {
    this->path_to_logger_file = path_to_logger_file;
  }
}