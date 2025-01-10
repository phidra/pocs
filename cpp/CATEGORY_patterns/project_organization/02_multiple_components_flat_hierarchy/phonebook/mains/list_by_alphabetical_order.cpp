#include <iostream>
#include <string>

#include "phonebook/database.h"

int main(int argc, char* argv[]) {
    // on attend une liste de noms uniquement (les âges sont hardcodés à une valeur dummy) :
    DataBase db;
    for (int i = 1; i < argc; ++i) {
        std::string name(argv[i]);
        int dummy_age = 42;
        Person p(name, dummy_age);
        db.add_person(p);
    }

    // affichage des personnes par orde alphabétique :
    auto sorted_persons = db.get_persons_by_alphabetical_order();
    for (auto person : sorted_persons) {
        std::cout << person.name << std::endl;
    }

    return 0;
}
