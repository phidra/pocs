#include "person/person.h"

#include "age_checker.h"

Person::Person(std::string _name, int _age) : name{_name}, age{_age}, beverage{is_adult(age) ? "BEER" : "SODA"} {}
