#include <algorithm>

#include "phonebook/database.h"
#include "filter.h"

void DataBase::add_person(Person person, Address address, Phone phone) {
    // do nothing if forbidden :
    if (is_forbidden(person.name)) {
        return;
    }

    if (person.age >= 100) {
        nb_of_very_old_persons += 1;
    }
    persons.push_back(std::make_tuple(person, address, phone));
}

std::vector<Person> DataBase::get_persons_by_insertion_order() const {
    std::vector<Person> to_return;
    for (auto& [person, address, phone] : persons) {
        to_return.push_back(person);
    }
    return to_return;
}

std::vector<Person> DataBase::get_persons_by_alphabetical_order() const {
    auto to_return = get_persons_by_insertion_order();
    std::sort(to_return.begin(), to_return.end(),
              [](Person const& left, Person const& right) { return left.name < right.name; });
    return to_return;
}
