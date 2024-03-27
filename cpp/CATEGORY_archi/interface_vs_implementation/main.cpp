#include <iostream>
#include <string>
#include <vector>

#include "Greeter.h"
#include "GreeterFactory.h"

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = le découplage de l'utilisation d'une interface avec son implémentation.

LES FICHIERS :
    Greeter.h n'expose qu'une classe virtuelle pure (interface) IGreeter, permettant de saluer plusieurs personnes
    main.cpp utilise GreeterFactory pour créer un Greeter concret (implémentant IGreeter)
    main.cpp utilise le greeter concret pour saluer plusieurs personnes
    il existe plusieurs implémentations de IGreeter (simple, threaded et reversed)
    GreeterFactory permet de créer chacune de ces implémentations, et d'y donner accès via un unique_ptr<IGreeter>


QUELLES SONT LES DÉPENDANCES :
    main.cpp :
        dépend de Greeter pour l'interface IGreeter
        dépend de GreeterFactory pour construire un IGreeter sans dépendre d'une de ses classes concrètes
        NE DÉPEND PAS d'un GreeterImpl !
    GreeterFactory :
        HEADER = dépend de Greeter (vu qu'il renvoie un objet qui implémente l'interface IGreeter)
        CPP    = dépend des GreeterImplXXX (vu qu'il est en charge de les construire)
        CPP    = dépend de Greeter (via son header)
    GreeterImplXXX :
        HEADER = dépend de Greeter (vu qu'il implémente l'interface IGreeter)
        CPP    = ne dépend de rien (juste de Greeter, via son header)
    TL;DR :
        tout le monde dépend de l'interface IGreeter (ce qui est logique)
        l'implémentation de la factory dépend des implémentations concrètes de IGreeter
        le main NE DÉPEND PAS des implémentations concrètes de IGreeter, il n'a connaissance que de IGreeter (et de la factory)


LE POINT CRUCIAL :
    le main ne dépend d'aucune implémentation concrète de IGreeter !
    du coup, si on veut en AJOUTER UNE NOUVELLE (par exemple, une qui greete dans un ordre randomisé)...
    ...le main n'a pas besoin d'être modifié, vu qu'il n'a AUCUNE connaissance des implémentations concrètes de IGreeter !
    (en revanche, il faut modifier l'implémentation de la factory, pour ajouter la dépendance + ajouter l'instanciation du nouveau greeter)

(c'est secondaire, mais pour bien montrer le découplage : le choix du greeter à utiliser n'est fait que tardivement, au runtime...
... alors même que le main ne dépend pas des implémentations des greeters)

)DELIM";
    std::cout << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    // Choic of greeter is chosen at runtime !
    std::cout << "Which greeter would you like to use :" << std::endl;
    std::cout << "1 = SIMPLE" << std::endl;
    std::cout << "2 = MULTITHREADED" << std::endl;
    std::cout << "3 = REVERSED ORDER" << std::endl;
    std::cout << "Your choice> " << std::flush;
    std::string entered;
    std::cin >> entered;
    std::cout << std::endl;

    std::unique_ptr<IGreeter> greeter;
    if (entered == "1") {
        greeter = create_greeter(GreeterType::Simple);
    } else if (entered == "2") {
        greeter = create_greeter(GreeterType::Threaded);
    } else if (entered == "3") {
        greeter = create_greeter(GreeterType::Reversed);
    } else {
        std::cout << "Dude... you must enter '1' or '2' or '3'..." << std::endl;
        return 1;
    }

    greeter->greet_many({"Luke", "Leia", "Yoda", "Palpatine", "Maul", "Vader"});
    return 0;
}
