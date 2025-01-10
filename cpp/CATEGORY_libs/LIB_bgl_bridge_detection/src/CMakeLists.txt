cmake_minimum_required(VERSION 3.0)
set(BIN connectedcomponents)
project($(BIN))
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

# boost libraries are pretty incompatible with flags : -Wall -Wextra -Werror...
# but when using system's boost, these checks are disabled by clang (so it compiles)
# when using conan's boost, these checks are not disabled, and compilation fails :-(
# using a clang specific option to ensure that checks are disabled on boost headers :
# see : https://stackoverflow.com/questions/44824874/clang-returns-different-compiler-errors-for-system-include-files
# see : https://clang.llvm.org/docs/UsersManual.html#controlling-diagnostics-in-system-headers
set(CMAKE_CXX_FLAGS "--system-header-prefix=boost/")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Release)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB MY_SOURCES "${CMAKE_SOURCE_DIR}" "*.cpp")
add_executable(${BIN} "${MY_SOURCES}")
target_link_libraries(${BIN} bz2 z expat -pthread)
target_include_directories(${BIN} PUBLIC ${CMAKE_SOURCE_DIR})
