#include "raii.h"

void atexit_handler() {
    std::cout << "We are about to exit, and still no RAII destructor will be called...\n" << std::flush;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    RAII pouet;
    // RAII does NOT work with std::exit.
    // cf. https://en.cppreference.com/w/cpp/utility/program/exit
    // Stack is not unwound: destructors of variables with automatic storage duration are not called.

    std::atexit(atexit_handler);
    std::exit(1);
}
