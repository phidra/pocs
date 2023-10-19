#include "group.h"

#include <iostream>

#include "person.h"
#include "name_modifier.h"
#include "announce.h"

Group::Group(Person _leader) : leader{_leader} {
    std::cout << announce_new_group() << "its leader is : " << modify_name(leader.name) << std::endl;
}

void Group::add_member(Person new_member) {
    std::cout << "New member : " << new_member.introduce() << std::endl;
    members.push_back(new_member);
}
