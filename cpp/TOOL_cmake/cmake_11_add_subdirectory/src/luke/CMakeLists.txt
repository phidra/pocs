# in opposition to include, here, path is resolved relatively to this current CMakeLists.txt
add_executable(luke main.cpp ../lib/lib.cpp)

# we can use a variable set in the parent CMakeLists.txt :
target_compile_definitions(luke PUBLIC RELATION="son of ${chosenone}")
target_include_directories(luke PUBLIC ../lib)

# As PARENT_SCOPE is not used, this variable will NOT be usable from parent CMakeLists.txt
SET (meaning_life_luke "HARMONY")
