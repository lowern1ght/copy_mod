cmake_minimum_required(VERSION 3.22)
project(copy_mod)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES
        ../src/*.cpp
        ../src/headers/*.h
        ../src/modules/*.cpp)

include_directories(modules headers)

add_executable(copy_mod ${SOURCES})

find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    include_directories(${OpenSSL_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenSSL_LIBRARIES})
endif ()

set_target_properties(PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)