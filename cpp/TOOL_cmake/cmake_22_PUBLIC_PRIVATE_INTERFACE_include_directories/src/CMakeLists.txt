# cette POC est une dérivée de la précédente (avec include_directories plutôt que compile_definitions) :

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# on utilise INTERFACE ici -> sublib1 n'utilisera pas l'include lui-même :
add_library(sublib1 sublib1/sublib1.cpp)
target_include_directories(sublib1 INTERFACE "src/includes/include1")

# on utilise INTERFACE ici -> sublib1 n'utilisera pas l'include lui-même :
add_library(sublib2 sublib2/sublib2.cpp)
target_include_directories(sublib2 INTERFACE "src/includes/include2")

# target 'lib' qui avec PUBLIC pour sublib1 et PRIVATE pour sublib2 :
add_library(lib lib/lib.cpp)
target_link_libraries(lib PUBLIC sublib1)
target_link_libraries(lib PRIVATE sublib2)


# L'exécutable est utilisateur de `lib` :
add_executable(bin main.cpp)
target_link_libraries(bin lib)
