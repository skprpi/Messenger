CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(m2m_chat_service "main.cpp")

file(GLOB M2M_CHAT_SOURCES "*.h" "*.cpp")
target_sources(m2m_chat_service PRIVATE ${M2M_CHAT_SOURCES})


# core
# include("${CMAKE_CURRENT_SOURCE_DIR}/../../core/CMakeLists.txt")
# target_sources(m2m_chat_service PRIVATE ${CORE_SOURCES})
target_include_directories(m2m_chat_service PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)


# boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.74.0 REQUIRED COMPONENTS system regex)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(m2m_chat_service ${Boost_LIBRARIES})

