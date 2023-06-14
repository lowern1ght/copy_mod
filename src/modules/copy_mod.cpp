//
// Created by lowern1ght on 06.06.2023.
//

#include <string>
#include <thread>
#include <copy_mod.h>
#include <filesystem>

using std::string;
using namespace std::filesystem;
using namespace std::chrono_literals;

unsigned long long in_folder_to_size = 0;

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

void copy_mod::loading_animation(const bool &working, const std::exception_ptr *exc_p, logger &logger, path &path_to, path &path_from) {
  using std::vector;
  using namespace indicators;

  std::cout
    << "\n"
    << termcolor::bold << termcolor::bright_yellow << " processing... "<< termcolor::reset
    << "\n";

  show_console_cursor(false);

  int copy_total_count = 1;
  unsigned long long copy_files_size = 0;

  if (is_directory(path_to)) {
    copy_total_count = 0;

    for (const auto& entity : recursive_directory_iterator(path_from)) {
      copy_files_size += entity.file_size();
      copy_total_count += 1;
    }

    ProgressBar pgrsbar{
        option::BarWidth{50},
        option::Start{"["},
        option::End{"]"},
        option::ForegroundColor{Color::white},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
    };

    //size_t copy_complete_count = 0;
    auto last_percent = 0;
    unsigned long long byte_current = 0;

    while(true) {
      byte_current = 0;
      for (const auto& entity : recursive_directory_iterator(path_to)) {
        for (const auto& entity_from : recursive_directory_iterator(path_from)) {
          if (entity.path().filename().string() == entity_from.path().filename().string()) {
            byte_current += entity.file_size();
          }
        }
      }

      if (exc_p != nullptr) { //if exception in main thread, throw exception and break loading bar;
        show_console_cursor(true);

        std::cout << termcolor::bold << termcolor::red
                  << " - copy failed!\n" << termcolor::reset;

        std::rethrow_exception(*exc_p);
      }

      auto n1 = (float)byte_current / (float)copy_files_size * 100;
      auto current_percent = static_cast<int>(std::round(n1));

      pgrsbar.set_option(option::PostfixText{ std::to_string(current_percent) + "%" });
      pgrsbar.set_progress(current_percent);

      if (copy_files_size == byte_current /* && copy_total_count == copy_complete_count*/) {
        pgrsbar.set_option(option::PostfixText { "file complete copy to" + path_to.string() });

        std::cout << termcolor::bold << termcolor::green
                  << " - copy successes!\n" << termcolor::reset;

        break;
      }

      //timeout
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  }

  std::cout << '\n';

  // Show cursor
  show_console_cursor(true);
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