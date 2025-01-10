#include <iostream>

#include "myapp.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    MyApp myapp;
    std::cout << "AGE = " << myapp.get_age("Luke", "Skywalker") << std::endl;
}
