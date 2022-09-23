cmake_minimum_required(VERSION 3.0.2)

project(poc)

set(SOURCES src/main.cpp)

add_executable(bin ${SOURCES})

# we need to use "add_custom_target" :
set(definition_file ${CMAKE_CURRENT_SOURCE_DIR}/src/value.h)
add_custom_target(
    building_definition_file
    COMMAND echo "This custom command will fail in 5..."
    COMMAND echo "This custom command will fail in 4..."
    COMMAND echo "This custom command will fail in 3..."
    COMMAND echo "This custom command will fail in 2..."
    COMMAND echo "This custom command will fail in 1..."
    COMMAND echo "This custom command will fail in 0..."
    COMMAND false  # <----- this is what is tested : this failing command does fail the entire build
    COMMAND echo "Just failed (but this echo should never appear)"
    VERBATIM
)
add_dependencies(bin building_definition_file)
