cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

set(BIN osm2boostgraph)
project($(BIN))
set(CMAKE_BUILD_TYPE Release)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${BIN}
    main.cpp
    common/polygon.cpp
    parsing/polygonfile.cpp
    graph/graph.cpp
    graph/osmparsing.cpp
    dumping/geojson.cpp
)
target_link_libraries(${BIN} bz2 z expat -pthread)
target_include_directories(${BIN} PUBLIC ${CMAKE_SOURCE_DIR})
