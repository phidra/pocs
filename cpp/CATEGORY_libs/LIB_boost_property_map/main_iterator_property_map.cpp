#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <iostream>
#include <map>
#include <string>

// Doc générale = https://www.boost.org/doc/libs/1_74_0/libs/property_map/doc/property_map.html
//
// Le code est inspiré de cet exemple :
//      https://www.boost.org/doc/libs/1_74_0/libs/property_map/doc/iterator_property_map.html
//
// Ce qu'on veut faire = associer des âges à des personnes.
// Le conteneur des âges sera un std::array
// Derrière, on veut pouvoir récupérer l'âge d'une personne à partir de son nom.
//
// Ce qui va se passer :
//      0. le conteneur étant un std::array, on n'est théoriquement pas capable de retrouver un âge autrement qu'à
//      partir d'un size_t (l'index de l'âge)
//      1. mais là intervient la property-map, et son OffsetMap (ce qu'en dit la doc : the value type must be
//      convertible to the difference type of the iterator)
//      2. étant donné une clé (nom d'une personne), on lit l'OffsetMap, pour récupérer l'offset où aller lire l'âge
//      dans le tableau, pour la personne concernée
//         d'une certaine façon, ce comportement ressemble aux hashmaps.
//         En effet, dans une hashmap, la clé est quelconque, mais les éléments sont stockés dans un tableau indexé par
//         des entiers. Il faut donc convertir la clé en un entier (l'index), pour une hashmap, c'est la fonction de
//         hachage qui fait cette conversion. Dans l'iterator property-map, on n'a pas de fonction de hachage, mais on a
//         une table de conversion : l'OffsetMap
//
// C'est apparemment très utilisé dans la BGL, pour associer des propriétés "externes" à un graphe existant :
//          boost::make_iterator_property_map(parents.begin(), get(boost::vertex_index, graph))
//          boost::make_iterator_property_map(edges.begin(), get(boost::edge_index, graph))
//
// Je pense que l'intérêt, c'est quand on dispose DÉJÀ d'un tableau stockant des objets (e.g. un graphe stocké en tant
// qu'adjacency_list avec un vector) Dans ce cas, la property-map est un Adapter, présentant une interface de type
// dictionnaire au dessus du vector pré-existant "contenant" le graphe. Et l'intérêt des PropertyMap pour la BGL, c'est
// qu'on dispose déjà des données (le vector des vertex du graphe), et de l'offset-map (le vertex_index du graphe)

struct Age {
    int age;
    Age(int x) : age{x} {}
};

std::ostream& operator<<(std::ostream& out, Age const& x) {
    out << x.age;
    return out;
}

int main(void) {
    // Le tableau statique sur lequel on va construire la property-map :
    std::array<Age, 4> my_array = {Age{22}, Age{22}, Age{45}, Age{58}};

    // L'offset-map permettant d'associer une clé (le nom d'une personne) à son offset dans le std::array :
    std::map<std::string, size_t> my_offset_map;
    my_offset_map["luke"] = 0;
    my_offset_map["leia"] = 1;
    my_offset_map["anakin"] = 2;
    my_offset_map["obi-wan"] = 3;

    // cette associative_property_map est "juste" un moyen d'implémenter l'OffsetMap.
    // (elle complique la compréhension pour rien, mais je n'arrive pas à simplement utiliser std::map)
    boost::associative_property_map<std::map<std::string, size_t> > my_offset_map_that_works(my_offset_map);

    // Construction de la property-map, à partir du tableau (contenant les properties), et de la map associant une clé à
    // un indes dans le tableau :
    auto pmap = boost::make_iterator_property_map(my_array.begin(), my_offset_map_that_works);

    // À partir de maintenant, pmap me permet d'avoir une interface de type dictionnaire (get/put utilisant des clés de
    // type string) sur un std::array :-)
    std::cout << "Valeur associée à luke = " << boost::get(pmap, "luke") << std::endl;
    std::cout << "Valeur associée à anakin = " << boost::get(pmap, "anakin") << std::endl;
    std::cout << "Changement de anakin pour lui attribuer un âge démesuré" << std::endl;
    boost::put(pmap, "anakin", Age{999});
    std::cout << "La nouvelle valeur de anakin est : " << boost::get(pmap, "anakin") << std::endl;
}
