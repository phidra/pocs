#include "GreeterImplThreaded.h"

#include <iostream>
#include <thread>

static void greet_one(std::string person) {
    std::cout << "Greetings, " << person << std::endl;
}

void GreeterImplThreaded::greet_many(std::vector<std::string> persons) const {
    std::vector<std::thread> threads;
    for (auto const& person : persons) {
        threads.emplace_back(greet_one, person);
    }

    for (auto& t : threads) {
        t.join();
    }
}
