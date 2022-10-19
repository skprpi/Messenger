cmake_minimum_required(VERSION 3.16)
project(core_tests CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-14")

# files
add_executable(core_fuzz_test)
target_sources(core_fuzz_test PRIVATE test_fuzz_router.cpp)

# corelib sources
target_include_directories(core_fuzz_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../src/include)
target_include_directories(core_fuzz_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../src/private)

# fuzz testing
target_compile_options(core_fuzz_test PRIVATE -g -fsanitize=fuzzer)
target_link_libraries(core_fuzz_test PRIVATE -fsanitize=fuzzer)
