#include "GreeterImplSimple.h"

#include <iostream>

void GreeterImplSimple::greet_many(std::vector<std::string> persons) const {
    for (auto const& person : persons) {
        std::cout << "Greetings, " << person << std::endl;
    }
}
