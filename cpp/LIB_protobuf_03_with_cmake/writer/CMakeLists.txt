cmake_minimum_required(VERSION 3.12)
project(writer CXX)

# this is actually needed by conan's version of libprotobuf :
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../addressbook.proto)
add_executable(writer main_writer.cpp ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(writer PUBLIC CONAN_PKG::protobuf)
target_include_directories(writer PRIVATE ${CMAKE_BINARY_DIR})
