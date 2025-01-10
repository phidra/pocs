#include <iostream>

#include "../lib/lib.h"

#ifndef RELATION
#define RELATION "WUT LUKE ?!"
#endif

int main(int argc, char* argv[]) {
    std::cout << std::endl;
    std::cout << "Luke is : " << RELATION << std::endl;
    display();
    std::cout << std::endl;
    return 0;
}
