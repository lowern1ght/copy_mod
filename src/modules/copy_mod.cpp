//
// Created by lowern1ght on 06.06.2023.
//

#include <string>
#include <conio.h>
#include <thread>
#include <copy_mod.h>
#include <filesystem>

using namespace std;
using namespace filesystem;

copy_exception::copy_exception(std::string &msg) throw()
  : exception(msg.c_str()) { }

const char *copy_exception::what() const {
  return exception::what();
}



void copy_mod::check_values_hash(path* pth_to, path* pth_from) {
  config->logger->write_message(" **** Begin check HASH *** \n", info);

  return; // developing...

  if (is_directory(*pth_to)) {
    for (auto entity_from : recursive_directory_iterator(*pth_from)) {
      for (auto entity_to : recursive_directory_iterator(*pth_to)) {
        cout << hash_value(entity_from.path()) << " | " << hash_value(entity_to.path()) << '\n';
      }
    }
  }
  else {
    cout << hash_value(*pth_to) << " | " << hash_value(*pth_from) << '\n';
  }

}

int get_count_elements(path* path_to_entity) {
  auto sum = 0;

  if (is_directory(*path_to_entity)) {
    for (auto entity : recursive_directory_iterator(*path_to_entity))
      sum += 1;
  }
  else {
    sum = 1;
  }

  return sum;
}

string get_directory_name(path* pth)
{
  string path_str = pth->string();
  auto found = path_str.find_last_of("/\\");
  string dir_name = pth->string().substr(found+1, path_str.length());

  return dir_name;
}

void copy_mod::start_copy()
{
  if (is_directory(*config->to_copy)) {
    auto name_dir = get_directory_name(config->from_entity_copy);
    config->to_copy = new path(*config->to_copy / name_dir);
  }

  auto sum_ents = get_count_elements(config->from_entity_copy);

  auto that_copy = is_directory(*config->from_entity_copy)
      ? "folder " + get_directory_name(config->from_entity_copy)
      : "file " + config->from_entity_copy->filename().string();

  this->config->logger->write_message("--- Begin copy (" + that_copy + ")", info);

  copy(*config->from_entity_copy, *config->to_copy, copy_options::overwrite_existing | copy_options::recursive);

  if (config->check_hash) {
    check_values_hash(config->from_entity_copy, config->to_copy);
  }
}

copy_mod::copy_mod(copy_config *config) {
  this->config = config;
}