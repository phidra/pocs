cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall")

project(poc_boost_geometry)
set(BIN poc_boost_geometry)

#set(CMAKE_CXX_COMPILER g++-9)
#set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_executable(${BIN} main.cpp)
target_link_libraries(${BIN} ${Boost_LIBRARIES})
