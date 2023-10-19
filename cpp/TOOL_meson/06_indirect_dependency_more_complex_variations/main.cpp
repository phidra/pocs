#include <iostream>

#include "group.h"
#include "person.h"

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = une situation complexe où le main a des dépendances indirectes + les commandes ninja.

Cette POC est la suite des deux précédentes, et introduit des situations plus complexes :
    - 'group' a maintenant une dépendance PUBLIQUE 'person' et une dépendance PRIVÉE 'name_modifier'
    - 'name_modifier' utilise à la fois des includes PRIVÉS et PUBLICS (seuls ces derniers sont incluables par un utilisateur de la lib)
    - 'group' a à la fois des includes PRIVÉS et PUBLICS (seuls ces derniers sont incluables par l'utilisateur de la lib = le main)

L'objectif de la POC est d'observer les impacts de tout ceci sur les commandes de compilation et de link utilisées par ninja.


EXEMPLE TYPIQUE DE SORTIE NINJA :
    [1/10] ccache c++ -Ilibperson.a.p -I. -I.. -I../person -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wpedantic -std=c++20 -g -fPIC -MD -MQ libperson.a.p/person_person.cpp.o -MF libperson.a.p/person_person.cpp.o.d -o libperson.a.p/person_person.cpp.o -c ../person/person.cpp
    [2/10] ccache c++ -Ilibname_modifier.a.p -I. -I.. -I../name_modifier -I../name_modifier_private -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wpedantic -std=c++20 -g -fPIC -MD -MQ libname_modifier.a.p/name_modifier_name_modifier.cpp.o -MF libname_modifier.a.p/name_modifier_name_modifier.cpp.o.d -o libname_modifier.a.p/name_modifier_name_modifier.cpp.o -c ../name_modifier/name_modifier.cpp
    [3/10] ccache c++ -Ilibname_modifier.a.p -I. -I.. -I../name_modifier -I../name_modifier_private -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wpedantic -std=c++20 -g -fPIC -MD -MQ libname_modifier.a.p/name_modifier_private_add_exclamation.cpp.o -MF libname_modifier.a.p/name_modifier_private_add_exclamation.cpp.o.d -o libname_modifier.a.p/name_modifier_private_add_exclamation.cpp.o -c ../name_modifier_private/add_exclamation.cpp
    [4/10] ccache c++ -Ilibgroup.a.p -I. -I.. -I../group -I../group_private -I../person -I../name_modifier -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wpedantic -std=c++20 -g -fPIC -MD -MQ libgroup.a.p/group_group.cpp.o -MF libgroup.a.p/group_group.cpp.o.d -o libgroup.a.p/group_group.cpp.o -c ../group/group.cpp
    [5/10] rm -f libperson.a && gcc-ar csrDT libperson.a libperson.a.p/person_person.cpp.o
    [6/10] rm -f libname_modifier.a && gcc-ar csrDT libname_modifier.a libname_modifier.a.p/name_modifier_name_modifier.cpp.o libname_modifier.a.p/name_modifier_private_add_exclamation.cpp.o
    [7/10] ccache c++ -Ilibgroup.a.p -I. -I.. -I../group -I../group_private -I../person -I../name_modifier -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wpedantic -std=c++20 -g -fPIC -MD -MQ libgroup.a.p/group_private_announce.cpp.o -MF libgroup.a.p/group_private_announce.cpp.o.d -o libgroup.a.p/group_private_announce.cpp.o -c ../group_private/announce.cpp
    [8/10] ccache c++ -Ilauncher.p -I. -I.. -I../group -I../person -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wpedantic -std=c++20 -g -MD -MQ launcher.p/main.cpp.o -MF launcher.p/main.cpp.o.d -o launcher.p/main.cpp.o -c ../main.cpp
    [9/10] rm -f libgroup.a && gcc-ar csrDT libgroup.a libgroup.a.p/group_group.cpp.o libgroup.a.p/group_private_announce.cpp.o
    [10/10] c++  -o launcher launcher.p/main.cpp.o -Wl,--as-needed -Wl,--no-undefined -Wl,--start-group libgroup.a libperson.a libname_modifier.a -Wl,--end-group


LES POINTS IMPORTANTS DANS LA SORTIE NINJA :
    - l'include du répertoire "name_modifier_private" n'est utilisé que pour :
        la compilation de 'name_modifier.cpp' et  'add_exclamation.cpp'
        c'est bien ce qui était attendu : cet include est privé à la librairie 'name_modifier' : on ne veut le voir nulle part ailleurs
        notamment, 'group' n'inclut PAS 'name_modifier_private', comme attendu
    - similairement, le répertoire "group_private" n'est inclus que pour compiler les deux cpp de la librairie 'group'
    - l'include du répertoire 'name_modifier' n'est utilisé que pour :
        la compilation de 'name_modifier.cpp' et 'add_exclamation.cpp' (normal, c'est la lib elle-même)
        la compilation de 'group.cpp' et 'announce.cpp' (normal, 'group' dépend des includes publics de 'name_modifier')
        le point important : la compilation de main.cpp N'INCLUT PAS le répertoire "name_modifier"
        ici aussi c'est bien ce qu'on souhaite : 'group' dépend de 'name_modifier' de façon PRIVÉE...
        ...il est donc normal que les utilisateurs de 'group' ne voient pas 'name_modifier'
    - le link du binaire 'launcher' utilise bien les fichiers .a des 3 librairies : group + person + name_modifier
        ce qui est intéressant, c'est que le link du launcher utilise aussi les dépendances INDIRECTES = libperson.a + libname_modifier.a
        (alors même que dans le fichier meson, le launcher ne dépend que de 'group')
        ça montre bien que meson a résolu les dépendances de façon transitive, ce qui était souhaité
        et pour les .a, à la différence des includes, il n'y a pas de notion de dépendance PUBLIQUE ou PRIVÉE
        par exemple, comme 'group' fait appel à une fonction de 'name_modifier', le code-objet de group.o aura une instruction qui...
        ... appelera un code-objet de libname_modifier.a ; du coup, comme main.o appelle le code-objet de group.o, il aura bien
        besoin lui aussi du code-objet de libname_modifier.a, même si la dépendance de group à name_modifier était PRIVÉE.

TL;DR :
    - utiliser un objet dépendance permet bien d'en récupérer les includes nécessaires
    - les includes publics et privés se comportent bien comme attendus
    - meson résout également correctement les dépendances indirectes vers les code-objets = les libxxx.a
    - pour les code-objets, il n'y a pas de notion de PUBLIC vs. PRIVATE
    - du coup, même si 'group' dépend de 'name_modifier' de façon privée, le link du launcher nécessitera bien libname_modifier.a

----
)DELIM";
    std::cout << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    Person chef{"Yoda", 800};
    Group jedis{chef};

    Person padawan{"Anakin", 10};
    Person knight{"Obiwan", 30};
    jedis.add_member(knight);
    jedis.add_member(padawan);
}
