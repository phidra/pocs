cmake_minimum_required(VERSION 3.0.2)

project(poc CXX)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wunused-parameter")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # explicitly ENABLING compile_commands.json

add_executable(bin src/main.cpp)
target_include_directories(bin PUBLIC "${CMAKE_SOURCE_DIR}/included")
