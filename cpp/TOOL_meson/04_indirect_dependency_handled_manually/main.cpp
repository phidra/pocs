#include <iostream>

#include "group.h"
#include "person.h"

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = une situation où le main a une dépendance indirecte + sa gestion MANUELLE dans meson.build

SITUATION :
- on a une première librairie 'person'
- on a une deuxième librairie 'group', qui dépend de 'person' PUBLIQUEMENT
  (i.e. les struct de la lib 'person' apparaissent dans les headers publics de la lib 'group')
- le main utilise la librairie 'group' (et utilise donc aussi 'person', vu que 'group' dépend de 'person' publiquement)

GESTION MANUELLE DANS LE FICHIER DE CONFIG MESON :
- TARGET 1 = on définit une première target de type "librairie statique" = person
  elle utilise ses propres includes (répertoire "person") et ne dépend de personne
- TARGET 2 = on définit une deuxième target de type "librairie statique" = group
  elle dépend de 'person' (avec le keyword 'link_with')
  elle utilise ses propres includes (répertoire "group"), mais également ceux de 'person'
  (c'est normal : la lib group utilise la lib person, elle a donc besoin d'inclure les structures de person)
- TARGET 3 = la target main dépend de la lib group (donc indirectement de la lib person) avec un 'link_with'
  la target main utilise à la fois les includes de 'group' et ceux de 'person'

Cette gestion manuelle fait le taf pour notre cas simple, mais pose problème à plus grande échelle :
- le problème est que la target main doivent inclure les includes de person !
- en effet, comme main ne dépend que de group, les includes de person (qui sont nécessaires à group car...
  ... group en dépend publiquement) devraient "venir avec" ceux de group, sans que main ait à les inclure explicitement
- pour pousser le bouchon un peu loin, si on gardait cette gestion manuelle des dépendances indirectes, un main
  complexe qui aurait un arbre de dépendances volumineux se retrouverait à devoir spécifier les chemins d'includes
  de TOUTES les feuilles de son arbre de dépendance !
- la situation souhaitée est donc plutôt que la dépendance de 'group' à 'person' soit transitive pour 'main' : si 'main'
  dépend de 'group', alors il n'a besoin d'inclure que les includes de 'group', et "gagne" automatiquement les includes
  de 'person' en tant que dépendance de 'group'.

À noter que pour voir les commandes exactes de compilation ET DE LINK, compile_commands.json ne suffit pas, on peut :
    cd meson_build_dir
    (ninja -t clean)
    ninja -v

La prochaine POC montre comment mieux gérer la dépendance indirecte de main à person
Celle encore après montre des variations un peu plus complexe, avec des dépendances privées et publiques.
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
