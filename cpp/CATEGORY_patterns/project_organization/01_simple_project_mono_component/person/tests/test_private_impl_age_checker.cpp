#include <iostream>

#include "age_checker.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    bool is_adult_1 = is_adult(100);
    if (!is_adult_1) {
        std::cerr << "UNEXPECTED is_adult_1" << std::endl;
        return 1;
    }

    bool is_adult_2 = is_adult(8);
    if (is_adult_2) {
        std::cerr << "UNEXPECTED is_adult_2" << std::endl;
        return 1;
    }

    return 0;
}
