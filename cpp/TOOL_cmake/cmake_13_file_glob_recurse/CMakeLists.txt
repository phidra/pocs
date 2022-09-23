cmake_minimum_required(VERSION 3.0.2)

project(poc-of-glob-recurse CXX)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wunused-parameter")
include(display_list.cmake)

# From now on, we will use GLOB_RECURSE to find source to build.

# by default, GLOB_RECURSE produces absolute paths :
file(
    GLOB_RECURSE
    ABSOLUTE_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# it is possible to produce relative paths :
file(
    GLOB_RECURSE
    RELATIVE_SOURCES
    RELATIVE "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

display_list("ABSOLUTE_SOURCES")
display_list("RELATIVE_SOURCES")

# this executable doesn't use the result of GLOB_RECURSE :
add_executable(
    bin1
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/modules/module1.cpp
    ${CMAKE_SOURCE_DIR}/src/modules/submodules/module2.cpp
    ${CMAKE_SOURCE_DIR}/src/modules/submodules/deeper/module3.cpp
)

# these executable uses results of GLOB_RECURSE :
add_executable(bin2 ${ABSOLUTE_SOURCES})
add_executable(bin3 ${RELATIVE_SOURCES})
