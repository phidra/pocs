cmake_minimum_required(VERSION 3.0)

project(poc_ncurses)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic")

add_executable(poc_ncurses main.cpp)
target_link_libraries(
    poc_ncurses
    ncurses
)
