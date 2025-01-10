#pragma once

#include <vector>

#include "person/person.h"

struct DataBase {
    void add_person(Person person);
    std::vector<Person> get_persons_by_insertion_order() const;
    std::vector<Person> get_persons_by_alphabetical_order() const;

    std::vector<Person> persons;
    int nb_of_very_old_persons = 0;
};
