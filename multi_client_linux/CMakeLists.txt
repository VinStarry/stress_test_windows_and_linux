cmake_minimum_required(VERSION 3.12)
project(multi_client_linux)

set(CMAKE_CXX_STANDARD 14)

add_executable(multi_client_linux main.cpp client.cpp client.h)

find_package(Threads REQUIRED)
target_link_libraries(multi_client_linux Threads::Threads)