#include <iostream>

// pour ne pas m'embêter avec les includes :
void lib();

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = les différences entre PUBLIC/PRIVATE/INTERFACE pour les compile_definitions.

Le comportement théorique attendu est défini dans le CMakeLists.txt.
Tout concerne le fait qu'une target ait ou non un flag défini à la compilation (-DFLAG) :

- sublib1 n'a ni FLAG1 ni FLAG2
- sublib2 n'a ni FLAG1 ni FLAG2
- lib a à la fois FLAG1 et FLAG2
- main n'a que FLAG1, et pas FLAG2

En pratique, ce n'est pas ce que j'observe : le main est compilé avec les DEUX flags... :-(

Aussi bien l'affichage des flags au runtime que le compile_commands.json montre que :

- les sublibs sont compilées SANS AUCUN flags (c'est bien le comportement attendu)
- lib est compilée AVEC les DEUX flags (c'est bien le comportement attendu)
- main est compilé AVEC les DEUX flags (ça n'est PAS le comportement attendu)

(et je ne sais pas où je me suis trompé)

)DELIM";
    std::cout << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    lib();

    std::cout << std::endl;
    std::cout << "===== comment a été compilé le main ?" << std::endl;
#ifdef FLAG1
    std::cout << "MAIN> FLAG1 est défini, yay !" << std::endl;
#else
    std::cout << "MAIN> FLAG1 n'est pas défini..." << std::endl;
#endif

#ifdef FLAG2
    std::cout << "MAIN> FLAG2 est défini, yay !" << std::endl;
#else
    std::cout << "MAIN> FLAG2 n'est pas défini..." << std::endl;
#endif
}
