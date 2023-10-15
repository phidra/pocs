#include <iostream>

// pour ne pas m'embêter avec les includes :
void lib();

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = les différences entre PUBLIC/PRIVATE/INTERFACE pour les include_directories.

Cette POC est un dérivée de la précédente, mais avec include_directories au lieu de compile_definitions.

Ici, c'est le compile_commands.json qui nous intéresse, pour voir quelles targets sont compilées avec quels -I

Et comme pour la POC précédente, je n'obtiens pas le comportement attendu :

- les sublibs sont compilées SANS AUCUN includes (c'est bien le comportement attendu)
- lib est compilée AVEC les DEUX includes (c'est bien le comportement attendu)
- main est compilé AVEC les DEUX includes (ça n'est PAS le comportement attendu)

Le comportement attendu était que le main soit compilé uniquement avec l'include1.
(et je ne sais pas où je me suis trompé)

)DELIM";
    std::cout << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    lib();

    std::cout << "main" << std::endl;
}
