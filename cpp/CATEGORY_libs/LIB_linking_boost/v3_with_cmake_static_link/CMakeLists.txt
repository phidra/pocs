cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

project(bin)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS program_options log log_setup)

# BOOST_LOG_DYN_LINK is only needed if dynamic link, see : https://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/log/installation/config.html
add_definitions("-UBOOST_LOG_DYN_LINK")

set(SRCS
    main.cpp)


add_executable(bin ${SRCS})
target_link_libraries(bin ${Boost_LIBRARIES})

