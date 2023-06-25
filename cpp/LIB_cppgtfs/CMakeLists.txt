cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

set(BIN bin)
project(poc_cppgtfs)
set(CMAKE_BUILD_TYPE Release)

add_subdirectory (cppgtfs)

add_executable(${BIN} main.cpp)
target_link_libraries(${BIN} ad_cppgtfs)
target_include_directories(${BIN} PUBLIC "cppgtfs")
