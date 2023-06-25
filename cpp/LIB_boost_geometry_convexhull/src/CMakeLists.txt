cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "-Wno-deprecated-copy")  # needed to compile boost

set(BIN convexhull)
project($(BIN))
set(CMAKE_BUILD_TYPE Release)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${BIN}
    main.cpp
)
#target_link_libraries(${BIN} bz2 z expat -pthread)
#target_include_directories(${BIN} PUBLIC ${CMAKE_SOURCE_DIR})
