cmake_minimum_required(VERSION 3.0.2)

project(poc)

set(SOURCES src/main.cpp)

add_executable(bin ${SOURCES})

# "add_custom_command" isn't sufficient here :
# NOTE : the custom_command is executed at build-time
add_custom_command(
    TARGET bin
    PRE_BUILD
    COMMAND echo "We can't use 'add_custom_command' here, as it doesn't enforce order"
    VERBATIM
)

# we need to use "add_custom_target" :
set(definition_file ${CMAKE_CURRENT_SOURCE_DIR}/src/value.h)
add_custom_target(
    building_definition_file
    COMMAND echo "Setting definition file..."
    COMMAND rm -f ${definition_file}
    COMMAND echo "#define VALUE1 42" > ${definition_file}
    COMMAND echo "Done"
    VERBATIM
)
add_dependencies(bin building_definition_file)
