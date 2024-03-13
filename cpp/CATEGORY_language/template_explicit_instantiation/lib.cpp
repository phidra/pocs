#include <iostream>
#include "lib.h"
#include "animals.h"

template <typename Animal>
void print_animal(Animal const& animal) {
    std::cout << animal.as_human_string() << std::endl;
}

template void print_animal(Cat const&);
template void print_animal(Dog const&);
