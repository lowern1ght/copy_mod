//
// Created by lowern1ght on 07.06.2023.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <logger.h>
#include <copy_mod.h>

using namespace std;
using namespace chrono;
using namespace filesystem;

logger::logger(path *path_to_logger_file, bool write_to_console = true, int rotation_size = 1073741824) {
  this->write_to_console = write_to_console;
  this->rotation_size = rotation_size;

  if (path_to_logger_file != nullptr) {
    this->path_to_logger_file = path_to_logger_file;
  }
}

string
get_enum_name(const logger_type type) {
  switch (type) {
  case logger_type::info:return "info";
    break;
  case logger_type::error:return "error";
    break;
  case logger_type::warning:return "warning";
    break;
  }

  return "";
}

void logger::check_log_file_size_and_clean() {
  if (exists(*path_to_logger_file) && file_size(*path_to_logger_file) >= rotation_size) {
    std::ofstream ofs;
    ofs.open(*path_to_logger_file, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
  }
}

void
logger::write_message(string message, logger_type type, bool pwrite_to_console) {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);

  std::ostringstream oss;
  oss << "[ " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] [ " + get_enum_name(type) << " ] " << message << '\n';

  string log_message = oss.str();

  if (type == error) {
    throw copy_exception(message);
  }

  if (write_to_console && pwrite_to_console) {
    //write only message without time and log type
    cout << message << '\n';
  }

  if (path_to_logger_file != nullptr) {
    check_log_file_size_and_clean();

    ofstream out_file(*path_to_logger_file, ios::app);
    out_file << log_message;
    out_file.close();
  }
}