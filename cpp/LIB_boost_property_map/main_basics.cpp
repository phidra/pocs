#include <iostream>
#include <map>
#include <string>
#include <boost/property_map/property_map.hpp>

// TL;DR : property_map = interface représentant un dictionnaire (i.e. une structure pouvant associer une valeur à une clé)
//
// Quel intérêt ?
//      certains algos doivent fetcher/setter les valeurs d'un dictionnaire (e.g. dijkstra, pour affecter un tentative_cost à un node)
//      grâce à property_map, ces algos n'ont pas à dépendre d'un type concret (std::map), et ne dépendent que d'une interface (la property_map).
//      cette interface peut être implémentée par plusieurs types, p.ex. std::map, un std::vector, etc.
//
// Le code est issu de cet exemple, que j'ai annoté en fonction de ma compréhenstion :
//      https://www.boost.org/doc/libs/1_74_0/libs/property_map/example/example1.cpp
//      (il y en a d'autres dans le répertoire parent de ce lien)
//
// doc = https://www.boost.org/doc/libs/1_74_0/libs/property_map/doc/property_map.html
//      The property map operations are global functions .
//      The interface for property maps consists of three functions: get(), put(), and operator[]
//      Invoking a property map function on an invalid key results in undefined behavior.
//      The need for property maps came out of the design of the Boost Graph Library,
//         whose algorithms needed an interface for accessing properties attached to vertices and edges in a graph.
//         In this context the vertex and edge descriptors are the key type of the property maps.
//
// On CONSTRUIT toujours une property_map à partir d'un conteneur concret.
// Il existe plein de conteneurs différents à partir desquels fabriquer une property-map.
// Chacune a une doc spécifique, listées dans la page de doc (p.ex. : https://www.boost.org/doc/libs/1_74_0/libs/property_map/doc/associative_property_map.html)
//      identity    : valeur = clé
//      associative : grosso-modo, c'est une std::map
//      iterator    : a priori, construit à partir d'un tableau (key=index)
//      vector      : a priori, comme iterator, mais à taille dynamique
//      [...]
//
// QUESTIONS :
//      Q = comment itérer sur les éléments d'une property_map ?
//      R = avec ma compréhension du problème, on peut pas, il faut utiliser la "vraie" structure (ici, std::map) directement.
//      Q = comment savoir si une clé est valide ?
//      R = avec ma compréhension du problème, on peut pas, il faut utiliser la "vraie" structure (ici, std::map) directement.


// Le point intéressant : foo ne dépend pas de std::map, mais uniquement de "quelque chose implémentant put/get/operator[]"
// Justement, property map est l'interface représentant ce "quelque chose implémentant put/get/operator[] (ici, à partir d'une std::map)
template <typename AddressMap>
void foo(AddressMap addresses)
{
  // boost::property_traits pour accéder aux types pertinent de la map :
  using value_type = typename boost::property_traits<AddressMap>::value_type;
  using key_type = typename boost::property_traits<AddressMap>::key_type;

  value_type old_address, new_address;
  key_type fred = "Fred";
  old_address = get(addresses, fred);  // get(map, key)
  new_address = "384 Fitzpatrick Street";
  put(addresses, fred, new_address);  // put(map, key, value)

  key_type joe = "John";
  value_type& address_of_joe = addresses[joe];  // map[key]
  address_of_joe = "325 Cushing Avenue";
}

void display(std::map<std::string, std::string> const& name2address)
{
  for (std::map<std::string, std::string>::const_iterator i = name2address.begin(); i != name2address.end(); ++i)
    std::cout << i->first << ": " << i->second << "\n";
}

int
main()
{
  // une map, vide pour le moment :
  std::map<std::string, std::string> name2address;

  // La property_map est construite sur une map vide :
  boost::associative_property_map< std::map<std::string, std::string> >
    address_map(name2address);

  name2address.insert(make_pair(std::string("Fred"), std::string("710 West 13th Street")));
  name2address.insert(make_pair(std::string("John"),  std::string("710 West 13th Street")));

  // Les adresses initiales :
  std::cout << "\nADDRESSES AVANT :" << std::endl;
  display(name2address);

  // La fonction foo ne travaille pas directement sur name2address, mais sur une property_map construite dessus.
  // Pourtant, elle va être capable de modifier les valeurs.
  foo(address_map);

  // Après appel de foo, on constate que les adresses dans la map (name2address) ont été modifiées (alors que foo a travaillé sur la property_map) :
  std::cout << "\nADDRESSES APRES :" << std::endl;
  display(name2address);

  return EXIT_SUCCESS;
}
