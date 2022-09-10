cmake_minimum_required(VERSION 3.16)
project(messenger CXX)

# set(CMAKE_CXX_COMPILER /usr/bin/g++) infinity loop in some cases
set(CMAKE_CXX_STANDARD 20)
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# googletest
add_subdirectory(./_third_party/googletest)
target_compile_definitions(gtest PUBLIC GTEST_LANG_CXX20 GTEST_HAS_TR1_TUPLE=0)

# services
add_subdirectory(./m2m_chat_service)
#add_subdirectory(./p2p_chat_service)
#add_subdirectory(./user_service)


# https://levelup.gitconnected.com/how-to-combine-c-cmake-googletest-circleci-docker-and-why-e02d76c060a3

