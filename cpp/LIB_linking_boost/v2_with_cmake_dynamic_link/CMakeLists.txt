cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

project(bin)

find_package(Boost COMPONENTS program_options log log_setup)

add_definitions("-DBOOST_LOG_DYN_LINK")

set(SRCS
    main.cpp)


add_executable(bin ${SRCS})
target_link_libraries(bin ${Boost_LIBRARIES})

