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

    bool should_loop = true;
    while (should_loop) {
        std::string name;
        std::cout << std::endl;
        std::cout << "Please enter a name (enter 'quit' or 'q' to exit) :" << std::endl;
        std::cin >> name;
        if (name == "quit" || name == "q") {
            std::cout << "Exiting early" << std::endl;
            std::cout << "Exiting early" << std::endl;
            std::cout << "Exiting early" << std::endl;
            return 0;
        }
        int age;
        std::cout << "Please enter " << name << "'s age :" << std::endl;
        std::cin >> age;

        Person p(name, age);
        db.add_person(p);

        if (db.get_persons_by_alphabetical_order().size() >= 3) {
            should_loop = false;
        }
    }

    auto persons = db.get_persons_by_alphabetical_order();
    std::cout << std::endl;
    std::cout << "There are " << persons.size() << " persons in the phonebook" << std::endl;
    std::cout << "Amongst them, here is the number of VERY OLD people : " << db.nb_of_very_old_persons << std::endl;
    std::cout << "My duty is done, bye..." << std::endl;

    return 0;
}
