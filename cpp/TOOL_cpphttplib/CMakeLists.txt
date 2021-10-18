cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

set(BIN echo)
project($(BIN))
set(CMAKE_BUILD_TYPE Release)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${BIN}
    main.cpp
)
target_link_libraries(${BIN} -pthread)
