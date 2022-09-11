cmake_minimum_required(VERSION 3.16)
project(messenger CXX)

# set(CMAKE_CXX_COMPILER /usr/bin/g++) infinity loop in some cases
set(CMAKE_CXX_STANDARD 20)
set(MICROSERVICES_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# googletest
add_subdirectory(${MICROSERVICES_ROOT_DIR}/_third_party/googletest googletest_build)
target_compile_definitions(gtest PUBLIC GTEST_LANG_CXX20 GTEST_HAS_TR1_TUPLE=0)

add_subdirectory(./tests)
add_subdirectory(./app)
