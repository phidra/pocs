#include <iostream>

#include "lib.h"
#include "animals.h"

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = l'instantiation explicite de template.

La syntaxe est un peu contre-intuitive :

- on déclare un template dans un header, mais sans donner son implémentation
- l'implémentation du template est privée, dans un cpp
- le même cpp propose des instantations explicites :
    template void print_animal(Cat const&);
    template void print_animal(Dog const&);
- tout se passe alors comme si on avait déclaré publiquement (et défini dans le cpp) ces deux fonctions :
    void print_animal(Cat const&);
    void print_animal(Dog const&);
- toute utilisation du template avec d'autres classes que Cat et Dog échouera !

NDM : le template n'est alors pas vraiment générique !
      Il ne marche que pour Cat et Dog, mais pas pour une troisième struct Bird, par exemple
      (ce que la présente POC illustre si on décommente le code ci-dessous)

cf. Explicit instantiation ici : https://en.cppreference.com/w/cpp/language/function_template

Quel intérêt ?
cf. https://stackoverflow.com/questions/2351148/explicit-template-instantiation-when-is-it-used

> If you define a template class that you only want to work for a couple of explicit types.

gagner en compile-time + output size
    en effet, le template n'est pas instantié dans chaque translation unit où il est utilisé
    il n'est instantié que dans une seule translation unit
ne pas mettre l'implémentation du header dans le template
    en effet, elle est dans le cpp

À ne pas confondre avec la [template specialization](https://en.cppreference.com/w/cpp/language/template_specialization)

)DELIM";
    cout << endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    Cat cat;
    Dog dog;
    [[maybe_unused]] Bird bird;
    print_animal(cat);
    print_animal(dog);

    // si décommenté, ceci ne compilera pas :
    // main.cpp:(.text+0x5b) : référence indéfinie vers « void print_animal<Bird>(Bird const&) »
    // print_animal(bird);
}
