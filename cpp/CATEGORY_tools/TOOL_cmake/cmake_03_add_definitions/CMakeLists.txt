cmake_minimum_required(VERSION 3.0.2)

project(poc-of-external-project)

# first way to add definition :
add_definitions(-DINTVALUE=42)

set(SOURCES src/main.cpp)

add_executable(bin ${SOURCES})

# second way to add definition :
target_compile_definitions(bin PUBLIC STRINGVALUE="coucou")
