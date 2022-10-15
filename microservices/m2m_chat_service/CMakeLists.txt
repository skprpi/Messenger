cmake_minimum_required(VERSION 3.16)
project(messenger CXX)

set(CMAKE_CXX_STANDARD 20)

# subdirs
add_subdirectory(./tests)
add_subdirectory(./app)
