#include <iostream>

// pour ne pas m'embêter avec les includes :
void sublib1();
void sublib2();

void lib() {
    std::cout << "lib" << std::endl;
    sublib1();
    sublib2();
}
