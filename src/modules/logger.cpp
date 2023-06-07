//
// Created by lowern1ght on 07.06.2023.
//

#include <ios>
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>
#include <iostream>
#include <logger.h>
#include <copy_mod.h>
#include <sstream>

using namespace std;
using namespace chrono;
using namespace filesystem;

logger::logger(path *path_to_logger_file, bool write_to_console = true) {
  this->write_to_console = write_to_console;

  if (path_to_logger_file != nullptr)
  {
    this->path_to_logger_file = path_to_logger_file;
  }
}

string get_enum_name(const logger_type type) {
  switch (type) {
  case logger_type::info:
    return "info";
    break;
  case logger_type::error:
    return "error";
    break;
  case logger_type::warning:
    return "warning";
    break;
  }

  return "";
}

void logger::write_message(string message, logger_type type) {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);

  std::ostringstream oss;
  oss << "[ " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] [ " + get_enum_name(type) << " ] " << message << '\n';

  string log_message =  oss.str();

  if (type == error)
  {
    throw copy_exception(message);
  }

  if (write_to_console)
  {
    cout << log_message << '\n';
  }

  if (filesystem::exists(*path_to_logger_file))
  {
    ofstream out_file(*path_to_logger_file, ios::app);
    out_file << log_message;
    out_file.close();
  }
}