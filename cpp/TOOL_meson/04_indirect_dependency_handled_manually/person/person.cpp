#include <string>

#include "person.h"

Person::Person(std::string _name, unsigned int _age) : name{_name}, age{_age} {}

std::string Person::introduce() const {
    auto to_return = std::string("Hello, my name is ");
    to_return += name;
    to_return += std::string(" and I am ");
    to_return += std::to_string(age);
    to_return += std::string(" years old.");
    return to_return;
}
