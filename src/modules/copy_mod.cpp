//
// Created by lowern1ght on 06.06.2023.
//

#include <string>
#include <conio.h>
#include <thread>
#include <colors.h>
#include <copy_mod.h>
#include <filesystem>

using namespace std;
using namespace filesystem;

copy_exception::copy_exception(std::string &msg) throw()
    : exception(msg.c_str()) {
}

const char *
copy_exception::what() const {
  return exception::what();
}

void
copy_mod::check_values_hash(path *pth_to, path *pth_from) {
  config->logger->write_message(" **** Begin check HASH *** \n", info);

  return; // developing...

  if (is_directory(*pth_to)) {
    for (auto entity_from : recursive_directory_iterator(*pth_from)) {
      for (auto entity_to : recursive_directory_iterator(*pth_to)) {
        cout << hash_value(entity_from.path()) << " | " << hash_value(entity_to.path()) << '\n';
      }
    }
  } else {
    cout << hash_value(*pth_to) << " | " << hash_value(*pth_from) << '\n';
  }

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
copy_mod::loading_animation(const bool &working, exception_ptr *exc_p, logger &logger) {
  cout << '\n' << " --- Copy  ";

  while (working) {
    this_thread::sleep_for(0.8s);
    cout << "\b\\" << flush;
    this_thread::sleep_for(0.8s);
    cout << "\b|" << flush;
    this_thread::sleep_for(0.8s);
    cout << "\b/" << flush;
    this_thread::sleep_for(0.8s);
    cout << "\b-" << flush;
  }

  try {
    if (exc_p != nullptr)
      rethrow_exception(*exc_p);
    logger.write_message("copy is COMPLETE", info, false);
    cout << "\b " << FGRN("COMPLETE") << " ---" << "\n";
  }
  catch (exception &exception) {
    logger.write_message("copy is FAILED with _message_ " + to_string(*exception.what()), info, false);
    cout << "\b " << FRED("ERROR") << ", " << exception.what() << " ---" << "\n";
  }
}

void
complete_copy(bool &working, path &path_from, path &path_to, exception_ptr *exception_ptr) {
  working = true;

  try {
    filesystem::copy(path_from, path_to, copy_options::overwrite_existing | copy_options::recursive);
  }
  catch (...) {
    *exception_ptr = current_exception();
  }

  working = false;
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

  this->config->logger->write_message(" --- Begin copy (" + that_copy + ") to " + config->to_copy->string(), info);

  auto working = true;
  exception_ptr *exc_ptr = nullptr;

  vector<thread> threads;

  threads.emplace_back(thread(complete_copy,
                              std::ref(working),
                              ref(*config->from_entity_copy),
                              ref(*config->to_copy),
                              exc_ptr));
  threads.emplace_back(thread(loading_animation, ref(working), exc_ptr, ref(*config->logger)));

  for (auto &th : threads)
    th.join();

  if (config->check_hash) {
    check_values_hash(config->from_entity_copy, config->to_copy);
  }
}

copy_mod::copy_mod(copy_config *config) {
  this->config = config;
}