#pragma once

#include <vector>

#include "person.h"

struct Group {
    Group(Person _leader);

    void add_member(Person new_member);

    Person leader;
    std::vector<Person> members;
};
