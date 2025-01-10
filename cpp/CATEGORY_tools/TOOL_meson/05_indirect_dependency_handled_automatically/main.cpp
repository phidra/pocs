#include <iostream>

#include "group.h"
#include "person.h"

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = une situation où le main a une dépendance indirecte + sa gestion AUTOMATIQUE dans meson.build

Cette POC est la suite de la précédente.

RAPPEL DE LA SITUATION :
- on a une première librairie 'person'
- on a une deuxième librairie 'group', qui dépend de 'person' PUBLIQUEMENT
  (i.e. les struct de la lib 'person' apparaissent dans les headers publics de la lib 'group')
- le main utilise la librairie 'group' (et utilise donc aussi 'person', vu que 'group' dépend de 'person' publiquement)


RAPPEL DU PROBLÈME :
- dans la POC précédente, on gérait manuellement la dépendance indirecte de 'main' à 'person' (en ajoutant une
  directive explicite dans le meson.build pour que 'main' ait connaissance des includes nécessaires à 'person')
- l'état souhaité est plutôt que 'main' ne doive gérer que ses dépendances DIRECTES, et que la résolution des includes
  nécessaires aux dépendances INDIRECTES se fasse automatiquement.
- (sinon, le meson.build d'un main complexe devrait gérer manuellement TOUTES les dépendances du main)

La présente POC résout ce problème :
- on utilise 'declare_dependency' pour créer un objet représentant la dépendance à la lib 'person'
- l'objet dépendance contient notamment les infos d'include
- le fait que 'group' dépende de 'person' est géré avec le keyword 'dependencies' qui pointe vers l'objet précédent
  (plutôt qu'avec 'link_with')
- du coup, il n'est plus nécessaire d'utiliser explicitement les includes de 'person' dans la target 'group', car ils
  viennent automatiquement avec l'objet représentant la dépendance à 'person'

À noter que pour voir les commandes exactes de compilation ET DE LINK, compile_commands.json ne suffit pas, on peut :
    cd meson_build_dir
    (ninja -t clean)
    ninja -v

À retenir = pour exprimer que 'group' dépend PUBLIQUEMENT de 'person', les includes de 'person' doivent faire partie
            des includes exposés par l'objet représentant la dépendance (créé avec 'declare_dependency').

La prochaine POC montre des variations un peu plus complexe, avec des dépendances privées et publiques.
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
