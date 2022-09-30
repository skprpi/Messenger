CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# unit tests
add_executable(m2m_chat_service_unit_test)
target_sources(m2m_chat_service_unit_test PRIVATE main.cpp)

# google test
find_package(GTest REQUIRED)
target_link_libraries(m2m_chat_service_unit_test PRIVATE GTest::GTest)
