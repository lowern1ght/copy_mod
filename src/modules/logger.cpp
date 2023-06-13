//
// Created by lowern1ght on 07.06.2023.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <logger.h>
#include <copy_mod.h>
#include <define_param.h>

#include "color.hpp"

using namespace std::chrono;
using namespace std::filesystem;

logger::logger(path *path_to_logger_file, bool write_to_console = true, long long rotation_size = 1073741824) {
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

  return DSTRING_EMPTY;
}

void
logger::check_log_file_size_and_clean() {
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

  if (write_to_console && pwrite_to_console) {
    log_to_console(message, type);
  }

  if (path_to_logger_file != nullptr) {
    check_log_file_size_and_clean();

    std::ofstream out_file(*path_to_logger_file, std::ios::app);
    out_file << log_message;
    out_file.close();
  }

  if (type == error) {
    throw copy_exception(message);
  }
}

void split_by_char(std::string const &str, const char* delim, std::vector<std::string> &out)
{
  char *token = strtok(const_cast<char*>(str.c_str()), delim);
  while (token != nullptr)
  {
    out.push_back(std::string(token));
    token = strtok(nullptr, delim);
  }
}

void logger::log_to_console(const std::string &str, logger_type type) {
  if (type == error) {
    std::cout << dye::colorize(str, "red") << "\n";
  }

  if (type == warning) {
    std::cout << dye::colorize(str, "yellow") << "\n";
  }

  if (type == info) {

    std::cout << str << '\n';
  }
}