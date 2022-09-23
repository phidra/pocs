#include <iostream>

#ifndef INTVALUE
#define INTVALUE "sadly, the intvalue is undefined :("
#endif

#ifndef STRINGVALUE
#define STRINGVALUE "sadly, the stringvalue is undefined :("
#endif

int main(int argc, char* argv[]) {
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "The stringvalue is... ... ...   " << STRINGVALUE << std::endl;
    std::cout << "The intvalue    is... ... ...   " << INTVALUE << std::endl;
    std::cout << std::endl;
    return 0;
}
