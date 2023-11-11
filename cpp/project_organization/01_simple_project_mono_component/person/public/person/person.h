#pragma once

#include <string>

struct Person {
    Person(std::string _name, int _age);

    std::string name;
    int age;
    std::string beverage;
};
