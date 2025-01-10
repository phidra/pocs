cmake_minimum_required(VERSION 3.0.2)

project(poc-of-external-project)

# path and size of "ls" binary is fetched at configure time :

# NOTE 1 : be careful with strings and quotes...
message(STATUS "---")
execute_process(COMMAND which ls OUTPUT_VARIABLE LS_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "The path of ls as seen by cmake is : ${LS_PATH}")
execute_process(COMMAND stat --printf=%s ${LS_PATH} OUTPUT_VARIABLE LS_SIZE OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "The size of ls as seen by cmake is : ${LS_SIZE}")

# NOTE 2 : we can pipe commands, by enqueuing them :
execute_process(COMMAND cat ${LS_PATH} COMMAND wc -l COMMAND cut -d\\  -f 1 OUTPUT_VARIABLE LS_NB_LINES OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "The nb of lines of ls as seen by cmake is : ${LS_NB_LINES}")

add_definitions(-DLS_PATH="${LS_PATH}")
add_definitions(-DLS_SIZE=${LS_SIZE})
add_definitions(-DLS_NB_LINES=${LS_NB_LINES})

set(SOURCES src/main.cpp)

add_executable(bin ${SOURCES})
