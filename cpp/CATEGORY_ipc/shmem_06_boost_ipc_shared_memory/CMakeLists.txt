cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

project(pouet)
add_executable(pouet main.cpp)
target_link_libraries(pouet pthread rt)
