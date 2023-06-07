cmake_minimum_required(VERSION 3.22)
project(copy_mod)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES
        src/*.cpp
        src/headers/*.h
        src/modules/*.cpp)

include_directories(src/modules src/headers)

add_executable(copy_mod ${SOURCES})

find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    include_directories(${OpenSSL_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenSSL_LIBRARIES})
endif ()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})