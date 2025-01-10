#include <iostream>

#include "value.h"

#ifndef VALUE1
#define VALUE1 "sadly, VALUE1 is undefined :("
#endif

int main(int argc, char* argv[]) {
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "VALUE1 is : " << VALUE1 << std::endl;
    std::cout << std::endl;
    return 0;
}
