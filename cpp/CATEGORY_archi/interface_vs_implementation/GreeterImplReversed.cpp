#include "GreeterImplReversed.h"

#include <algorithm>
#include <iostream>

void GreeterImplReversed::greet_many(std::vector<std::string> persons) const {
    std::reverse(persons.begin(), persons.end());
    for (auto const& person : persons) {
        std::cout << "Greetings, " << person << std::endl;
    }
}
