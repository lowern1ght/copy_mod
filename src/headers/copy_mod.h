//
// Created by lowern1ght on 06.06.2023.
//

#ifndef COPY_MOD_SRC_MODULES_COPY_MOD_H
#define COPY_MOD_SRC_MODULES_COPY_MOD_H

#include <iostream>
#include <stdlib.h>
#include "copy_config.h"

using namespace std;

class copy_mod {
private:
  copy_config* config;
private:
  void check_values_hash();
public:
  copy_mod(copy_config* config);
  void start_copy();
};

class copy_exception : public exception {
public:
  copy_exception(string &msg) throw();
  const char *what() const override;
};

#endif //COPY_MOD_SRC_MODULES_COPY_MOD_H
