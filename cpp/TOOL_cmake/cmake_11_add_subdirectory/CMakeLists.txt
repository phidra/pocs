cmake_minimum_required(VERSION 3.0.2)

project(mysuperpoc CXX)

# this variable is defined in parent CMakeLists.txt, and used in subdirectories :
set(chosenone "Anakin")

# targets are defined in included CMakeLists.txt :
add_subdirectory(src/luke)
add_subdirectory(src/leia)

# To make things defined in subdirectories VISIBLE, they must be defined with PARENT_SCOPE.
# To illustrate : this is the case for Leia, but not for Luke :
message(STATUS "What is the meaning of life according to Leia : ${meaning_life_leia}")
message(STATUS "What is the meaning of life according to Luke : ${meaning_life_luke}")

# NOTE : we CANNOT do the following :
# target_link_libraries(luke lib)
# target_link_libraries(leia lib)
#
# https://stackoverflow.com/questions/26838367/cmake-attempted-to-add-link-library-to-target-which-is-not-built-in-this-direct/36509508#36509508
# > The target_link_libraries directive needs to be part of the same CMakeLists.txt as the add_executable directive.
# > This also applies to libraries. Think of each directory and associated CMakeLists.txt as a subproject.
