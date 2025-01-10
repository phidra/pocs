cmake_minimum_required(VERSION 3.0.2)

project(mysuperpoc CXX)

# this variable is defined in parent CMakeLists.txt, but used in included XXX.cmake :
set(chosenone "Anakin")

# targets are defined in included XXX.cmake :
include(src/luke.cmake)
include(src/leia.cmake)
