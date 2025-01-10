# in opposition to include, here, path is resolved relatively to this current CMakeLists.txt
add_executable(leia main.cpp ../lib/lib.cpp)

# we can use a variable set in the parent CMakeLists.txt :
target_compile_definitions(leia PUBLIC RELATION="daughter of ${chosenone}")
target_include_directories(leia PUBLIC ../lib)

# The use of PARENT_SCOPE will ensure that this variable is usable from parent CMakeLists.txt
SET (meaning_life_leia "POWER" PARENT_SCOPE)
