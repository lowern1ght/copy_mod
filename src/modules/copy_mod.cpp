//
// Created by lowern1ght on 06.06.2023.
//

#include <string>
#include <thread>
#include <iostream>
#include <Windows.h>
#include <copy_mod.h>
#include <filesystem>

using std::string;
using namespace std::filesystem;

copy_exception::copy_exception(std::string &msg) throw()
    : exception(msg.c_str()) {
}

const char *
copy_exception::what() const {
  return exception::what();
}

void
copy_mod::check_values_hash(path *pth_to, path *pth_from) {
  config->logger->write_message("begin check HASH\n", info);
}

int
get_count_elements(path *path_to_entity) {
  auto sum = 0;

  if (is_directory(*path_to_entity)) {
    for (auto entity : recursive_directory_iterator(*path_to_entity))
      sum += 1;
  } else {
    sum = 1;
  }

  return sum;
}

string
get_directory_name(path *pth) {
  string path_str = pth->string();
  auto found = path_str.find_last_of("/\\");
  string dir_name = pth->string().substr(found + 1, path_str.length());

  return dir_name;
}

void
complete_copy(bool &working, path &path_from, path &path_to, std::exception_ptr *exception_ptr) {
  working = true;

  try {
    std::filesystem::copy(path_from, path_to, copy_options::overwrite_existing | copy_options::recursive);
  }
  catch (...) {
    *exception_ptr = std::current_exception();
  }

  working = false;
}

void copy_mod::loading_animation(const bool &working,
                                 std::exception_ptr *exc_p,
                                 logger &logger,
                                 path &path_to,
                                 path &path_from) {


  if (is_directory(path_from)) {
    int count_full = 0;
    for (auto ent : std::filesystem::recursive_directory_iterator(path_from)) {
      count_full += 1;
    }

    progressbar pgrbar(count_full);
    pgrbar.set_todo_char(" ");
    pgrbar.set_done_char("█");

    while(true) {
      std::vector<string> ready_copy;
      for (const auto& entity_to : std::filesystem::recursive_directory_iterator(path_to)) {
        if (std::find(ready_copy.begin(), ready_copy.end(), entity_to.path().string()) != ready_copy.end() )
          ready_copy.push_back(entity_to.path().string());
      }

      if (ready_copy.capacity() == count_full) {
        break;
      }
    }
  }
  else {
    progressbar pgrbar(1);
    pgrbar.set_todo_char(" ");
    pgrbar.set_done_char("█");

    auto path_result = path_to / path_from.filename();
    while (true) {
      if(exists(path_result)) {
        pgrbar.update();
        break;
      }
    }
  }



  while (working) {




  }
}


void
copy_mod::start_copy() {
  if (is_directory(*config->to_copy)) {
    auto name_dir = get_directory_name(config->from_entity_copy);
    config->to_copy = new path(*config->to_copy / name_dir);
  }

  auto that_copy = is_directory(*config->from_entity_copy)
                   ? "folder " + get_directory_name(config->from_entity_copy)
                   : "file " + config->from_entity_copy->filename().string();

  this->config->logger->write_message("begin copy \"" + that_copy + "\" to " + config->to_copy->string(), info);

  auto working = true;
  std::exception_ptr *exc_ptr = nullptr;

  std::vector<std::thread> threads;
  threads.emplace_back(std::thread(complete_copy,
                              std::ref(working),
                              std::ref(*config->from_entity_copy),
                              std::ref(*config->to_copy),
                              exc_ptr));

  threads.emplace_back(std::thread(loading_animation,
                                   std::ref(working),
                                   exc_ptr,
                                   std::ref(*config->logger),
                                   std::ref(*config->to_copy),
                                   std::ref(*config->from_entity_copy)));

  for (auto &th : threads)
    th.join();

  if (config->check_hash != nullptr && config->check_hash) {
    check_values_hash(config->from_entity_copy, config->to_copy);
  }
}

copy_mod::copy_mod(copy_config *config) {
  this->config = config;
}