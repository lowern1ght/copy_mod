#pragma pack copy_mod

#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <copy_mod.h>
#include <filesystem>
#include <copy_config.h>
#include <define_param.h>

const char SYMBOL_SEPARATE = '=';

using std::map;
using std::pair;
using std::string;
using std::iostream;
using std::exception;
using std::exception_ptr;
using namespace std::filesystem;

std::string
clear_string(const string &str, char symbol_remove = '"') {
  string result;

  for (auto const c : str) {
    if (c != symbol_remove) {
      result += c;
    }
  }

  return result;
}

std::pair<string, string> *
get_pair_from_string(const string& arg)
noexcept {
  string first, second;

  for (
      int i = 0;
      i < arg.
          length();
      ++i) {
    if (arg[i] == SYMBOL_SEPARATE) {
      first = arg.substr(0, i);
      if (i + 1 < arg.
          length()
          ) {
        second = arg.substr(i + 1, arg.length());
      }
    }
  }

  if (first.empty()) {
    first = arg;
  }

  if (first == PARAM_NAME_COPY_TO ||
      first == PARAM_NAME_COPY_FROM ||
      first == PARAM_NAME_CHECK_ON ||
      first == PARAM_NAME_ROTATION ||
      first == PARAM_NAME_LOG) {
    return new
        pair<string, string>(first, !second.empty() ?
                                    clear_string(second)
                                                         : second);
  } else {
    return
        nullptr;
  }
}

void
parse_arguments_to_map(int argc, char *argv[], map<string, string> &arguments)
noexcept {
  for (
      int i = 0;
      i < argc;
      ++i) {
    auto pair = get_pair_from_string(argv[i]);

    if (pair != nullptr) {
      arguments.
          insert(*pair);
    }
  }
}

string
to_upper(const string& str) {
  string str_t;
  for (char i : str) {
    str_t += toupper(i);
  }
  return str_t;
}

/// \param arg
/// \return
long long
get_int_from_argument(const string& arg) {
  if (arg == STRING_EMPTY) {
    return 0;
  }

  size_t type_dt = 1;

  auto upper_str = to_upper(arg); //String to upper

  auto find_gb = upper_str.find(ROTATION_GB);
  if (find_gb != std::string::npos) {
    upper_str.replace(upper_str.find(ROTATION_GB), sizeof(ROTATION_GB.size()) - 1, DSTRING_EMPTY);
  }

  auto find_mb = upper_str.find(ROTATION_MB);
  if (find_mb != std::string::npos) {
    upper_str.replace(upper_str.find(ROTATION_MB), sizeof(ROTATION_MB.size()) - 1, DSTRING_EMPTY);
    type_dt = 2;
  }

  auto find_kb = upper_str.find(ROTATION_KB);
  if (find_kb != std::string::npos) {
    upper_str.replace(upper_str.find(ROTATION_KB), sizeof(ROTATION_KB.size()) - 1, DSTRING_EMPTY);
    type_dt = 3;
  }

  string digits = STRING_EMPTY;
  for (auto ch : upper_str) {
    if (isdigit(ch)) {
      digits += ch;
    }
  }

  auto arg_int = (long long) stoi(digits);

  long long gb_convert = 1073741824;
  long long mb_convert = 1048576;
  long long kb_convert = 1024;

  long long result = NULL;

  switch (type_dt) {
  case 1:result = arg_int * gb_convert;
    break;
  case 2:result = arg_int * mb_convert;
    break;
  case 3:result = arg_int * kb_convert;
    break;
  default:break;
  }

  return result;
}

/// Parse arguments list to copy_config class
/// \param arguments
/// \param logger
/// \return copy_config
copy_config *
get_config_from_arguments(map<string, string> &arguments, logger *logger) {
  auto *config = new copy_config;

  // ===============================================================================================

  path *path_to_log_file = nullptr;
  long long rotation_max_size = 2097152; //default 2MB

  if (arguments.count(PARAM_NAME_LOG) == 1) {
    auto lg_file_value = arguments.find(PARAM_NAME_LOG)->second;

    if (lg_file_value.empty()) {
      lg_file_value = "log.txt";
    }

    path_to_log_file = new path(current_path() / lg_file_value);

    if (arguments.count(PARAM_NAME_ROTATION) == 1) {
      rotation_max_size = get_int_from_argument(arguments.find(PARAM_NAME_ROTATION)->second);
    }

    logger = new class logger(path_to_log_file, true, rotation_max_size);
    config->logger = logger;
  } else if (arguments.count(PARAM_NAME_LOG) > 1) {
    if (logger != nullptr) {
      logger->write_message("argument [" + PARAM_NAME_LOG + "] specified more than once.", error);
    }
  }

  if (logger == nullptr) {
    logger = new class logger(nullptr, true, rotation_max_size);
  }

  // ===============================================================================================

  path *path_from_copy = nullptr;

  if (arguments.count(PARAM_NAME_COPY_FROM) == 1) {
    try {
      path_from_copy = new path(arguments.find(PARAM_NAME_COPY_FROM)->second);
    }
    catch (exception const &exception) {
      logger->write_message("wrong path to _entity_", logger_type::error);
    }

    if (!std::filesystem::exists(*path_from_copy)) {
      logger->write_message("the creature that needs to be copied has not been found", error);
    }
  } else if (arguments.count(PARAM_NAME_COPY_FROM) > 1) {
    logger->write_message(" the creature that needs to be copied has not been found", error);
  }

  if (path_from_copy != nullptr) {
    config->from_entity_copy = path_from_copy;
  }

  // ===============================================================================================

  path *path_to_copy = nullptr;

  if (arguments.count(PARAM_NAME_COPY_TO) == 1) {
    path_to_copy = new path(arguments.find(PARAM_NAME_COPY_TO)->second);
    if (std::filesystem::exists(*path_to_copy)) {
      config->to_copy = path_to_copy;
    }
      //Todo: реализовать копирование в несколько папок (directories)
    else if (arguments.count(PARAM_NAME_COPY_TO) > 1) {
      logger->write_message("argument [" + PARAM_NAME_COPY_TO + "] specified more than once.", error);
    } else {
      logger->write_message("end point to copy is bad path", error);
    }
  }

  // ===============================================================================================

  bool check_hash_summary = false;
  if (arguments.count(PARAM_NAME_CHECK_ON) == 1) {
    config->check_hash = new bool(true);
  } else if (arguments.count(PARAM_NAME_CHECK_ON) > 1) {
    logger->write_message("argument [" + PARAM_NAME_CHECK_ON + "] specified more than once.", error);
  } else {
    config->check_hash = nullptr;
  }

  return config;
}

int
main(int argc, char *argv[]) {
  map<string, string> arguments; //key value a pair of arguments
  parse_arguments_to_map(argc, argv, arguments); //parse command line argument to map

  copy_config *config = nullptr;
  logger *logger = nullptr;

  try {
    config = get_config_from_arguments(arguments, logger); //get config
  }
  catch (exception const &error) {
    std::cout << error.what() << '\n';
    return EXIT_FAILURE;
  }

  copy_mod cpmod(config);
  cpmod.start_copy();

  return EXIT_SUCCESS;
}