#include <iostream>

#include "../lib/lib.h"

#ifndef RELATION
#define RELATION "WUT LEIA ?!"
#endif

int main(int argc, char* argv[]) {
    std::cout << std::endl;
    std::cout << "Leia is : " << RELATION << std::endl;
    display();
    std::cout << std::endl;
    return 0;
}
