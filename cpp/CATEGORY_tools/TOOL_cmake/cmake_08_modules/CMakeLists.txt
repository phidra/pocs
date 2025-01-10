cmake_minimum_required(VERSION 3.0)

project(compilation_with_cmake)

# setting dummy variables to show the use of "CMakePrintHelpers" module :
set(DIRECTION "north")
set(ANGLE "0°")

# manually displaying them :
message(STATUS "The direction is : ${DIRECTION}")
message(STATUS "The angle is : ${ANGLE}")

# here, use of 'cmake_print_variables' would fail, as module is not included yet :
# cmake_print_variables(DIRECTION ANGLE)


include(CMakePrintHelpers)
# here, use of 'cmake_print_variables' succeeds, as module has been included :
message(STATUS "The following line is the use of 'cmake_print_variables' :")
cmake_print_variables(DIRECTION ANGLE)


set(SRCS
    main.cpp
    lib.cpp
    )
    
add_executable(bin ${SRCS})
