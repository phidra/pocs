set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# on associe la compile_definition FLAG1 à la target `sublib1` (qui est une lib) avec le scope INTERFACE
# du coup, la lib sublib1 n'utilise pas elle-même ce flag (sauf erreur de ma part, elle sera compilée SANS le flag)
add_library(sublib1 sublib1/sublib1.cpp)
target_compile_definitions(sublib1 INTERFACE FLAG1)

# idem pour la target `sublib2` et FLAG2 :
add_library(sublib2 sublib2/sublib2.cpp)
target_compile_definitions(sublib2 INTERFACE FLAG2)

# on définit une target `lib` (qui est ici aussi une lib), avec deux choses intéressantes : upstream et downstream.
# upstream : la target doit linker avec les libs `sublib1` et `sublib2` ; le fait d'avoir utilisé `INTERFACE`
#            précédemment pour la compile_definition fait que `lib` est compilé AVEC les flags FLAG1 et FLAG2.
# downstream : la target a indiqué qu'elle linkait avec `sublib1` en `PUBLIC` et `sublib2` en `PRIVATE`.
#              Du coup, les utilisateurs de `lib` auront le flag FLAG1 mais pas FLAG2.
add_library(lib lib/lib.cpp)
target_link_libraries(lib PUBLIC sublib1)
target_link_libraries(lib PRIVATE sublib2)


# L'exécutable est utilisateur de `lib`, il devrait être buildé avec FLAG1 mais sans FLAG2 (EDIT : mais ça ne marche pas) :
add_executable(bin main.cpp)
target_link_libraries(bin lib)
