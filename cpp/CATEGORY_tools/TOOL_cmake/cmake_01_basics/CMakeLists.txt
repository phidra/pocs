cmake_minimum_required(VERSION 3.0)

project(compilation_with_cmake)

set(SRCS
    main.cpp
    lib.cpp
    )
    
add_executable(bin ${SRCS})
