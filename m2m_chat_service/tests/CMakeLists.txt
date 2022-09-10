CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# unit tests


add_executable(m2m_chat_service_unit_test)
target_sources(m2m_chat_service_unit_test PRIVATE main.cpp)

# target_compile_options(m2m_chat_service_unit_test PUBLIC cxx_strict)
# set_target_properties(m2m_chat_service_unit_test PROPERTIES COMPILE_FLAGS "${cxx_strict}")
target_link_libraries(m2m_chat_service_unit_test gtest gtest_main)
