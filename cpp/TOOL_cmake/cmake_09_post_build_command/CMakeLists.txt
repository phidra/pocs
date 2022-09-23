cmake_minimum_required(VERSION 3.0)

project(moving_binary_after_build)

set(SRCS main.cpp)
    
add_executable(mybinary ${SRCS})

add_custom_command(
    TARGET mybinary
    POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy mybinary ${CMAKE_CURRENT_SOURCE_DIR}
)

# grep "command-line tool mode" in the cmake's man to understand "-E" and "copy"
