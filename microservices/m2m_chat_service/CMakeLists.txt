cmake_minimum_required(VERSION 3.16)
project(messenger CXX)

# set(CMAKE_CXX_COMPILER /usr/bin/g++) infinity loop in some cases
set(CMAKE_CXX_STANDARD 20)

# subdirs
add_subdirectory(./tests)
add_subdirectory(./app)
# add_subdirectory(../core _build)


# boost
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.74.0 COMPONENTS system thread REQUIRED) 
include_directories(${Boost_INCLUDE_DIRS}) 
