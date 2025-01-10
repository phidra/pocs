#include <iostream>

#include "person/person.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    Person albus("Dumbledore", 110);
    if (albus.beverage != "BEER") {
        std::cerr << "ERROR> Albus should be allowed to drink beer !" << std::endl;
        return 1;
    }

    Person chosen_one("Harry", 10);
    if (chosen_one.beverage == "BEER") {
        std::cerr << "ERROR> Harry should NOT be allowed to drink beer, he's far too young !" << std::endl;
        return 1;
    }

    return 0;
}
