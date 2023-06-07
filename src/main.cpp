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
#include "copy_mod.h"

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
  string first = DSTRING_EMPTY, second = DSTRING_EMPTY;

  for (int i = 0; i < arg.length(); ++i) {
    if (arg[i] == SYMBOL_SEPARATE) {
      first = arg.substr(0, i);
      if (i + 1 < arg.length()) {
        second = arg.substr(i+1, arg.length());
      }
    }
  }

  if (first == DSTRING_EMPTY) {
    first = arg;
  }

  if (first == PARAM_NAME_COPY_TO    ||
      first == PARAM_NAME_COPY_FROM  ||
      first == PARAM_NAME_CHECK_ON   ||
      first == PARAM_NAME_LOG)
  {
    return new pair<string, string>(first, second != DSTRING_EMPTY ? clear_string(second) : second);
  }
  else {
    return nullptr;
  }
}

void parse_arguments_to_map(int argc, char *argv[], map<string, string> &arguments) noexcept {
  for (int i = 0; i < argc; ++i) {
    auto pair = get_pair_from_string(argv[i]);

    if (pair != nullptr) {
      arguments.insert(*pair);
    }
  }
}

copy_config *get_config_from_arguments(map<string, string> &arguments, logger* logger) {
  copy_config* config = new copy_config;

  // ===============================================================================================

  path* path_to_log_file = nullptr;

  if (arguments.count(PARAM_NAME_LOG) == 1) {
    auto value_pair = arguments.find(PARAM_NAME_LOG)->second;

    auto default_path = new path(current_path() / "log.txt");

    /*if (value_pair == DSTRING_EMPTY) {
      path_to_log_file = default_path;
    }
    else {
      auto temp_path = new path(value_pair);
      if (exists(temp_path->remove_filename()) && is_directory(temp_path->remove_filename())) {
        path_to_log_file = temp_path;
      }
      else if (temp_path->is_relative()) {
        path_to_log_file = new path(current_path() / temp_path->filename());
      }
      else {
        path_to_log_file = default_path;
      }
    }*/

    path_to_log_file = new path(current_path() / "log.txt");

    logger = new class logger(path_to_log_file, true);
    config->logger = logger;
  }
  else if(arguments.count(PARAM_NAME_LOG) > 1) {
    logger->write_message("*** Argument [\" + PARAM_NAME_LOG + \"] specified more than once.", error);
  }

  logger->write_message(" --- LOGGER ACTIVE --- ", info);

  // ===============================================================================================

  path* path_from_copy = nullptr;

  if (arguments.count(PARAM_NAME_COPY_FROM) == 1) {
    try {
      path_from_copy = new path(arguments.find(PARAM_NAME_COPY_FROM)->second);
    } catch(exception const &exception) {
      logger->write_message(" *** Wrong path to _entity_ *** ", logger_type::error);
    }

    if (!filesystem::exists(*path_from_copy)) {
      logger->write_message(" *** the creature that needs to be copied has not been found *** ", error);
    }
  }
  else if (arguments.count(PARAM_NAME_COPY_FROM) > 1) {
    logger->write_message(" *** the creature that needs to be copied has not been found *** ", error);
  }

  if (path_from_copy != nullptr) {
    config->from_entity_copy = path_from_copy;
  }

  // ===============================================================================================

  path* path_to_copy = nullptr;

  if (arguments.count(PARAM_NAME_COPY_TO) == 1) {
    path_to_copy = new path(arguments.find(PARAM_NAME_COPY_TO)->second);
    if (filesystem::exists(*path_to_copy)) {
      config->to_copy = path_to_copy;
    }
    //Todo: реализовать копирование в несколько папок (directories)
    else if (arguments.count(PARAM_NAME_COPY_TO) > 1) {
      logger->write_message(" *** Argument [" + PARAM_NAME_COPY_TO + "] specified more than once. *** ", error);
    }
    else {
      logger->write_message("*** End point to copy is bad path ***", error);
    }
  }

  // ===============================================================================================

  bool check_hash_summary = false;
  if (arguments.count(PARAM_NAME_CHECK_ON) == 1 ) {
    config->check_hash = new bool(true);
  }
  else if (arguments.count(PARAM_NAME_CHECK_ON) > 1) {
    logger->write_message("*** Argument [" + PARAM_NAME_CHECK_ON + "] specified more than once.", error);
  }
  else {
    config->check_hash = false;
  }

  return config;
}

int main(int argc, char *argv[]) {
  map<string, string> arguments; //key value a pair of arguments
  parse_arguments_to_map(argc, argv, arguments); //parse command line argument to map

  copy_config* config = nullptr;
  logger* logger = nullptr;

  try {
    config = get_config_from_arguments(arguments, logger); //get config
  }
  catch (exception const error) {
    cout << error.what() << '\n';
    return EXIT_FAILURE;
  }

  copy_mod cpmod(config);
  cpmod.start_copy();

  return EXIT_SUCCESS;
}