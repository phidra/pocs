#include <iostream>
#include <vector>

#include "glaze/glaze.hpp"

using namespace std;

namespace example {

struct City {
    string country;
    float density;
};

// pour cette structure, on définit nous-même la façon dont elle sera encodée :
struct Location {
    uint32_t number;
    string street;
    City city;
};

struct Person {
    string first_name;
    string last_name;
    uint32_t age;
    std::optional<City> dream_city;

    vector<int> preferred_numbers;              // vectors
    unordered_map<string, Location> relations;  // unordered_map indexed by strings + nested substructures
    // unordered_map<int, Person> friends;  // unordered_map indexed by ints + self-referencing
};

}  // namespace example

// défintion "manuelle" (i.e. sans utiliser la reflection) de comment on serialize Location :
// ATTENTION : ces template-specialiations semblent devoir être dans le namespace global :
template <>
struct glz::meta<example::Location> {
    using T = example::Location;
    static constexpr auto value = object("address without number",  // rename field
                                         &T::street,
                                         "city",
                                         &T::city);
};

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = un exemple de serialization json d'un objet plus complexe :

- nested structures
- vector
- unordered_map
- optional

Notamment : la lib est capable d'inspecter les structures : par défaut, on n'a pas besoin d'écrire du code
supplémentaire pour serializer une structure existante.

On peut pretty-printer une string json.

QUELQUES NOTES ET LOUPS :

- serialization des optional = lève un warning de compilation dans le code de glaze
- serialization d'une structure auto-référencée : casse la reflection par défaut
- si la lib fait des choses bizarres (et affiche p.ex. des structures vides), essayer de définir les métadonnées
  manuelles plutôt que se reposer sur la reflection.
- au final, seuls les cas simplistes permettent de profiter de la réflection (mais d'un autre côté, les métadonnées
  manuelles sont faciles à écrire)
- limtation : les métadonnées doivent être définies dans le namespace global...
- à cause du langage (ou d'une limitation de glaze ?), les métadonnées ne peuvent pas être définies dans une fonction
- si je veux profiter de la reflection et ne pas avoir à créer une structure supplémentaire, les structures ne doivent
  pas avoir de constructeur (je ne comprends pas bien pourquoi)


)DELIM";
    cout << endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    using namespace example;
    vector<Person> directory;

    City Paris{"France", 99.999};
    City Berlin{"Deutschland", 777.888};
    unordered_map<string, Location> john_locations = {
        {"pouet", Location{19, "rue de la Pouetterie", Paris}},
        {"youpi", Location{42, "rue de la Ferronerie", Berlin}},
    };
    directory.emplace_back(Person{"John", "Doe", 33, {Berlin}, {3, 12, 7}, john_locations});
    directory.emplace_back(Person{"Alice", "Right", 22, {}, {}, {}});

    string buffer{};
    glz::write_json(directory, buffer);

    cout << glz::prettify(buffer) << "\n\n";

    array<Person, 2> another_directory;
    auto error_code = glz::read_json(another_directory, buffer);

    if (error_code != glz::error_code::none) {
        cout << "Error reading buffer !" << endl;
        return 1;
    }

    string another_buffer{};
    glz::write_json(another_directory, another_buffer);

    if (buffer == another_buffer) {
        cout << "Directories are the same!\n";
    }

    return 0;
}
