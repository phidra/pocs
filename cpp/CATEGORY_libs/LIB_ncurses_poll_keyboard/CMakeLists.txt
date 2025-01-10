cmake_minimum_required(VERSION 3.0)

project(poll_keyboard)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic")

add_executable(poll_keyboard main.cpp)
target_link_libraries(poll_keyboard ncurses)
