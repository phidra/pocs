cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

project(mutex_god)
add_executable(mutex_god main.cpp)
target_link_libraries(mutex_god pthread rt)
