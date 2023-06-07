/*
 * application: copy_mod
 *
 * by lowern1ght
 *
 * */

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <copy_config.h>
#include <define_param.h>

#define STRING_EMPTY ""

const char SYMBOL_SEPARATE = '=';

using namespace std;
using namespace filesystem;

string clear_string(const string &str, char symbol_remove = '"') {
  string result;

  for (auto const c : str) {
    if (c != symbol_remove) { result += c; }
  }

  return result;
}

pair<string, string>* get_pair_from_string(const string arg) noexcept {
  string first = STRING_EMPTY, second = STRING_EMPTY;

  for (int i = 0; i < arg.length(); ++i) {
    if (arg[i] == SYMBOL_SEPARATE) {
      first = arg.substr(0, i);
      if (i + 1 < arg.length()) {
        second = arg.substr(i+1, arg.length());
      }
    }
  }

  if (first == PARAM_NAME_COPY_TO    ||
      first == PARAM_NAME_COPY_FROM  ||
      first == PARAM_NAME_CHECK_ON   ||
      first == PARAM_NAME_LOG)
  {
    return new pair<string, string>(first, clear_string(second));
  }
  else {
    return nullptr;
  }
}

void parse_arguments_to_map(int argc, char *argv[], map<string, string> &arguments) {
  for (int i = 0; i < argc; ++i) {
    auto pair = get_pair_from_string(argv[i]);

    if (pair != nullptr) {
      arguments.insert(*pair);
    }
  }
}

copy_config* get_config_from_arguments(map<string, string> &arguments) {
  copy_config* config = new copy_config;

  path* path_from_copy = nullptr;

  if (arguments.count(PARAM_NAME_COPY_FROM) == 1) {
    try {
      path_from_copy = new path(arguments.find(PARAM_NAME_COPY_FROM)->second);
    } catch(exception const &exception) {
      throw new std::exception("*** Wrong path to _entity_ ***");
    }

    if (!filesystem::exists(*path_from_copy)) {
      throw new std::exception(" *** the creature that needs to be copied has not been found *** ");
    }
  }

  if (path_from_copy != nullptr) {
    config->from_entity_copy = path_from_copy;
  }

  path* paths_to_copy = nullptr;

  if (arguments.count(PARAM_NAME_COPY_TO) == 1) {

  }

  if (arguments.count(PARAM_NAME_LOG) == 1) {
    auto value_pair = arguments.find(PARAM_NAME_LOG)->second;

    path* path_to_log_file = nullptr;
    if (value_pair == STRING_EMPTY) {
      path_to_log_file = new path(current_path() / "log.txt");
    }
    else {
      if (exists(value_pair) && !filesystem::is_directory(value_pair)) {
        path_to_log_file = new path(value_pair);
      }
      else {
        path_to_log_file = new path(current_path() / value_pair);
      }
    };

    logger* logger = new class logger(path_to_log_file, true);
    config->logger = logger;
  }

  return config;
}

int main(int argc, char *argv[]) {
  map<string, string> arguments; //key value a pair of arguments
  parse_arguments_to_map(argc, argv, arguments); //parse command line argument to map

  copy_config* config;

  try {
    config = get_config_from_arguments(arguments); //get config
  }
  catch (exception const error) {
    cout << error.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}