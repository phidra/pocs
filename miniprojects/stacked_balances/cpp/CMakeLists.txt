cmake_minimum_required(VERSION 3.0)

project(stacked_balances)

set(CMAKE_CXX_FLAGS "-Wall -pedantic -std=c++11")

add_executable(bin main.cpp)
