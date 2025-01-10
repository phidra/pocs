#pragma once

#include <tuple>
#include <vector>

#include "person/person.h"
#include "personal_infos/address/address.h"
#include "personal_infos/phone/phone.h"

struct DataBase {
    void add_person(Person person, Address address, Phone phone);
    std::vector<Person> get_persons_by_insertion_order() const;
    std::vector<Person> get_persons_by_alphabetical_order() const;

    std::vector<std::tuple<Person, Address, Phone>> persons;
    int nb_of_very_old_persons = 0;
};
