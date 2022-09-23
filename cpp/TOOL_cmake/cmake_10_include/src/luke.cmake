# it seems that path is defined relative to the parent CMakeLists.txt
# thus, we have to search for main_luke.cpp in "src" folder :
add_executable(luke src/main_luke.cpp)

# we can use a variable set in the parent CMakeLists.txt :
target_compile_definitions(luke PUBLIC RELATION="son of ${chosenone}")

