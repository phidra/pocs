#include <iostream>
#include <string>

#include "phonebook/database.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "===== Expecting 3 persons :" << std::endl;
    std::cout << "===========================" << std::endl;

    // ici, on peut jouer avec le fait que les noms commençant par M sont filtrés
    DataBase db;

    db.add_person(Person("Luke", 42), Address(12, "farm", "Tatooine"), Phone(12, 34));
    db.add_person(Person("Leia", 42), Address(99, "palace", "Aldeeran"), Phone(99, 99));
    db.add_person(Person("Mace", 55), Address(9, "council", "Coruscant"), Phone(0, 0));  // filtered
    db.add_person(Person("Yoda", 900), Address(8, "swamp", "Dagobah"), Phone(900, 900));

    auto persons = db.get_persons_by_alphabetical_order();
    std::cout << std::endl;
    std::cout << "There are " << persons.size() << " persons in the phonebook" << std::endl;
    std::cout << "Amongst them, here is the number of VERY OLD people : " << db.nb_of_very_old_persons << std::endl;
    std::cout << "My duty is done, bye..." << std::endl;

    return 0;
}
