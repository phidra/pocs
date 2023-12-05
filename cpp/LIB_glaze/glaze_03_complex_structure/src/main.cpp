#include <iostream>
#include <vector>

#include "glaze/glaze.hpp"

using namespace std;

namespace example {

// ce que je veux :
//      des sous-structures
//      des vectors
//      des unordered_map indexées par des entiers et des strings
//      des optional
//      self referencing

struct City {
    string country;
    float density;
};

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

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = un exemple de serialization json d'un objet plus complexe :

- nested structures
- vector
- unordered_map
- optional


Notamment : on n'a pas besoin d'écrire du code supplémentaire pour serializer une structure existante : par défaut la lib est capable d'inspecter les structures.

On peut pretty-printer une string json.

Deux petits loups :

- serialization des optional = lève un warning de compilation dans le code de glaze
- serialization d'une structure auto-référencée : casse la reflection par défaut

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
