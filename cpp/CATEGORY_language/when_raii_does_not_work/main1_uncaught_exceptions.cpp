#include "raii.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    RAII pouet;
    // RAII is not guaranteed to work with uncaught exceptions :
    // cf. https://en.cppreference.com/w/cpp/language/throw#Stack_unwinding
    // It is implementation-defined whether any stack unwinding takes place for uncaught exceptions.
    throw std::runtime_error("pouet");
}
