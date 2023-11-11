#include <iostream>

#include "personal_infos/address/address.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    auto a = Address{40, "rue du Vieux Cimetière", "DONALDVILLE"};
    if (a.number != 40 || a.street != "rue du Vieux Cimetière" || a.city != "DONALDVILLE") {
        std::cerr << "UNEXPECTED address" << std::endl;
        return 1;
    }

    return 0;
}
