# it seems that path is defined relative to the parent CMakeLists.txt
# thus, we have to search for main_leia.cpp in "src" folder :
add_executable(leia src/main_leia.cpp)

# we can use a variable set in the parent CMakeLists.txt :
target_compile_definitions(leia PUBLIC RELATION="daughter of ${chosenone}")

