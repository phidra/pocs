#include <iostream>

void sublib1() {
#ifdef FLAG1
    std::cout << "SUBLIB1> FLAG1 est défini, yay !" << std::endl;
#else
    std::cout << "SUBLIB1> FLAG1 n'est pas défini..." << std::endl;
#endif

#ifdef FLAG2
    std::cout << "SUBLIB1> FLAG2 est défini, yay !" << std::endl;
#else
    std::cout << "SUBLIB1> FLAG2 n'est pas défini..." << std::endl;
#endif
}
