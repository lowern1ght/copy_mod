//
// Created by lowern1ght on 06.06.2023.
//

#ifndef COPY_MOD_SRC_MODULES_COPY_MOD_H
#define COPY_MOD_SRC_MODULES_COPY_MOD_H

#include <cstdlib>
#include <iostream>
#include <copy_config.h>
#include <progressbar.hpp>

class copy_mod {
private:
  copy_config *config;
  static void
  loading_animation(const bool &working,
                                std::exception_ptr *exc_p,
                                logger &logger,
                                std::filesystem::path& path_to,
                                std::filesystem::path& path_from);
private:
  void
  check_values_hash(path *pth_to, path *pth_from);

public:
  explicit copy_mod(copy_config *config);

  void
  start_copy();
};

class copy_exception : public std::exception {
public:
  explicit copy_exception(std::string &msg) throw();

  [[nodiscard]] const char *
  what() const override;
};

#endif //COPY_MOD_SRC_MODULES_COPY_MOD_H
