# cmake for vscode

cmake_minimum_required(VERSION 3.16)
project(microservices CXX)

set(CMAKE_CXX_STANDARD 20)

# corelib
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/src/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/src/private)

# services
add_subdirectory(./m2m_chat_service)
