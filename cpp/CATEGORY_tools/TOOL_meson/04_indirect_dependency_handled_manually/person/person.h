#pragma once

#include <string>

struct Person {
    Person(std::string _name, unsigned int _age);
    std::string introduce() const;

    std::string name;
    unsigned int age;
};
