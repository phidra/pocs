#include <algorithm>

#include "phonebook/database.h"
#include "filter.h"

void DataBase::add_person(Person person) {
    // do nothing if forbidden :
    if (is_forbidden(person.name)) {
        return;
    }

    if (person.age >= 100) {
        nb_of_very_old_persons += 1;
    }
    persons.push_back(person);
}

std::vector<Person> DataBase::get_persons_by_insertion_order() const {
    return persons;  // persons are already sorted in insertion order
}

std::vector<Person> DataBase::get_persons_by_alphabetical_order() const {
    auto to_return = persons;
    std::sort(to_return.begin(), to_return.end(),
              [](Person const& left, Person const& right) { return left.name < right.name; });
    return to_return;
}
