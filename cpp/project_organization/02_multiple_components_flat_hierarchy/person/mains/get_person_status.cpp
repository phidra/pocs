#include <iostream>
#include <string>

#include "person/person.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "ERROR : missing args" << std::endl;
    }
    std::string name(argv[1]);
    int age = std::stoi(argv[2]);

    Person p(name, age);

    std::cout << p.name << " is " << p.age << " years old and drinks : " << p.beverage << std::endl;

    return 0;
}
