cmake_minimum_required(VERSION 3.0.2)

project(poc-of-fetch-content CXX)
set(BIN bin)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wunused-parameter")

# custom function to display FetchContent variables :
function(show_FetchContent_vars prefix lcName)
    message(STATUS "------------------------------------------------")
    message(STATUS "${prefix} : ${lcName}")
    message(STATUS "        ${lcName}_SOURCE_DIR = ${${lcName}_SOURCE_DIR}")
    message(STATUS "        ${lcName}_BINARY_DIR = ${${lcName}_BINARY_DIR}")
    message(STATUS "        ${lcName}_POPULATED  = ${${lcName}_POPULATED}")
    message(STATUS "")
endfunction()


include(FetchContent)

# This merely declares the 3 variables (but this DOES NOT download anything) :
FetchContent_Declare(
    MySuperCppGtfs 
    GIT_REPOSITORY "https://ad-git.informatik.uni-freiburg.de/ad/cppgtfs"
    GIT_TAG "master"
) 
FetchContent_Declare(
  gtfs-sample-data
  URL        https://github.com/google/transit/raw/master/gtfs/spec/en/examples/sample-feed-1.zip
  URL_HASH   MD5=4d9d3040c284f0581cd5620d5c131109
  SOURCE_DIR gtfs-sample-data
)

# To effectively download the content (at configure time), two options are possible :

# OPTION 1 = when no customisation is needed, use FetchContent_MakeAvailable :
# https://cmake.org/cmake/help/latest/module/FetchContent.html#command:fetchcontent_makeavailable
# This effectively downloads content (and if there is a CMakeLists.txt, calls add_subdirectory) :
FetchContent_MakeAvailable(gtfs-sample-data)
message(STATUS "")
execute_process(COMMAND echo "Now that gtfs data has been downloaded, what is the content of : ${gtfs-sample-data_SOURCE_DIR}")
execute_process(COMMAND ls -lh "${gtfs-sample-data_SOURCE_DIR}")
message(STATUS "")

# OPTION 2 = when we need to customize, we must use the so-called "canonical pattern" manually :
# https://cmake.org/cmake/help/latest/module/FetchContent.html#fetch-content-canonical-pattern
string(TOLOWER "MySuperCppGtfs" lcName)  # lcName lc = lower-case
show_FetchContent_vars("BEFORE FetchContent_GetProperties" "${lcName}")
FetchContent_GetProperties(MySuperCppGtfs)
show_FetchContent_vars("AFTER FetchContent_GetProperties" "${lcName}")

if(NOT ${lcName}_POPULATED)
    # This effectively fetches the content :
    FetchContent_Populate(MySuperCppGtfs)

    # This is the customization that we need : we have to patche the sources :
    execute_process(COMMAND sed -i "s@pow10@powersOf10@g" "${${lcName}_SOURCE_DIR}/src/ad/util/CsvParser.cpp")

    # Bring the populated content into the build
    add_subdirectory(${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR})
endif()
show_FetchContent_vars("AFTER FetchContent_Populate" "${lcName}")

# From now on, both content have been downloaded (and in the case of cppgtfs, is added to the _build)

add_executable(${BIN} src/main.cpp)
target_include_directories(${BIN} PUBLIC "${${lcName}_SOURCE_DIR}/src")
target_link_libraries(${BIN} ad_cppgtfs)
