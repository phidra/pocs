#include <iostream>

#include "personal_infos/phone/phone.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    auto p = Phone{40, 123};
    if (a.indicatif != 40 || a.number != 123) {
        std::cerr << "UNEXPECTED phone" << std::endl;
        return 1;
    }

    return 0;
}
