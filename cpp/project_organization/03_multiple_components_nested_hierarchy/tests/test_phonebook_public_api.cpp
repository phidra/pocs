#include <iostream>

#include "person/person.h"
#include "personal_infos/address/address.h"
#include "phonebook/database.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    DataBase db;

    db.add_person(Person("Yoda", 900), Address(99, "a", "b"), Phone(1, 2));
    db.add_person(Person("Luke", 32), Address(99, "a", "b"), Phone(1, 2));
    db.add_person(Person("Leia", 32), Address(99, "a", "b"), Phone(1, 2));
    db.add_person(Person("Mace", 55), Address(99, "a", "b"), Phone(1, 2));

    // Mace should be filtered out :
    auto persons = db.get_persons_by_insertion_order();
    if (persons.size() != 3) {
        std::cerr << "ERROR> wrong number of persons" << std::endl;
        return 1;
    }

    // Yoda should appear last when alphabetical order and first in insertion order :
    auto sorted = db.get_persons_by_alphabetical_order();
    if (persons.front().name != "Yoda") {
        std::cerr << "ERROR> wrong insertion order !" << std::endl;
        return 1;
    }
    if (sorted.back().name != "Yoda") {
        std::cerr << "ERROR> wrong alphabetical order !" << std::endl;
        return 1;
    }

    return 0;
}
