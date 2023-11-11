#include <iostream>

#include "filter.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    bool is_forbidden_1 = is_forbidden("Albus");
    if (is_forbidden_1) {
        std::cerr << "UNEXPECTED Albus should NOT be forbidden" << std::endl;
        return 1;
    }

    bool is_forbidden_2 = is_forbidden("Malefoy");
    if (!is_forbidden_2) {
        std::cerr << "UNEXPECTED Malefoy should be forbidden" << std::endl;
        return 1;
    }

    return 0;
}
