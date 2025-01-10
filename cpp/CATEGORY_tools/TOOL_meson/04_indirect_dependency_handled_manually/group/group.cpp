#include "group.h"

#include <iostream>

Group::Group(Person _leader) : leader{_leader} {
    std::cout << "WE HAVE A NEW GROUP, its leader is : " << leader.name << std::endl;
}

void Group::add_member(Person new_member) {
    std::cout << "New member : " << new_member.introduce() << std::endl;
    members.push_back(new_member);
}
