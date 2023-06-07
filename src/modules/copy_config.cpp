//
// Created by lowern1ght on 06.06.2023.
//

#include <logger.h>
#include <filesystem>

using namespace std;
using namespace filesystem;

class copy_config {
public:
    path* to_copy;
    path* from_entity_copy;
    logger* logger;
};