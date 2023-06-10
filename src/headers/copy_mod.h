//
// Created by lowern1ght on 06.06.2023.
//

#ifndef COPY_MOD_SRC_MODULES_COPY_MOD_H
#define COPY_MOD_SRC_MODULES_COPY_MOD_H

#include <iostream>
#include <cstdlib>
#include <copy_config.h>

using namespace std;

class copy_mod {
private:
  copy_config *config;

  static void
  loading_animation(const bool &working, exception_ptr *exc_p, logger &logger);
private:
  void
  check_values_hash(path *pth_to, path *pth_from);

public:
  explicit copy_mod(copy_config *config);

  void
  start_copy();
};

class copy_exception : public exception {
public:
  copy_exception(string &msg) throw();

  [[nodiscard]] const char *
  what() const override;
};

#endif //COPY_MOD_SRC_MODULES_COPY_MOD_H
