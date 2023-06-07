//
// Created by lowern1ght on 06.06.2023.
//

#include <bitset>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <openssl/md5.h>

using namespace std;

class controller {
public:
  std::string get_hash_summary(const std::filesystem::path entity);
  std::string get_byte_entity(std::filesystem::path entity);
};

std::string controller::get_hash_summary(const std::filesystem::path entity) {
  unsigned char hash[MD5_DIGEST_LENGTH];

  if (filesystem::is_directory(entity)) {
    for (auto entity : filesystem::recursive_directory_iterator()) {
      cout << "entity: " <<  entity.path().filename() << " | is directory: " << entity.is_directory() << '\n';
    }
  }
  else {
    auto bytes = get_byte_entity(entity);
  }


  return "";
}

std::string controller::get_byte_entity(std::filesystem::path entity) {

  std::ifstream ifs(entity.c_str(), std::ios::ate | std::ios::binary);
  if (!ifs.is_open()) {
    throw new std::exception("file or directory is couldn't open open");
  }

  std::string result;

  return result;
}