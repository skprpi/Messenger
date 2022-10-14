cmake_minimum_required(VERSION 3.16)
project(core_tests CXX)

set(CMAKE_CXX_STANDARD 20)

# files
add_executable(core_unit_test)
target_sources(core_unit_test PRIVATE test_router.cpp)
target_include_directories(core_unit_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..) # root folder (to include core)

# google test
find_package(GTest REQUIRED)
target_link_libraries(core_unit_test PRIVATE GTest::GTest)

# boost
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.74.0 COMPONENTS system thread regex REQUIRED) 
include_directories(${Boost_INCLUDE_DIRS})
