cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

set(POC rtree)
project($(POC))
set(CMAKE_BUILD_TYPE Release)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${POC} main.cpp)
target_link_libraries(${POC} bz2 z expat -pthread ${CONAN_LIBS})
