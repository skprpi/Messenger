CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(m2m_chat_service "main.cpp" )

file(GLOB P2P_CHAT_SOURCES "*.h" "*.cpp")
target_sources(m2m_chat_service PRIVATE ${P2P_CHAT_SOURCES})
