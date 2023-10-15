#include <iostream>

// pour ne pas m'embêter avec les includes :
void sublib1();
void sublib2();

void lib() {
    std::cout << std::endl;
    std::cout << "===== comment ont été compilées les sublibs ?" << std::endl;
    sublib1();
    sublib2();

    // voyons ce que lib connaît comme flag :
    std::cout << std::endl;
    std::cout << "===== comment a été compilée lib ?" << std::endl;
#ifdef FLAG1
    std::cout << "LIB> FLAG1 est défini, yay !" << std::endl;
#else
    std::cout << "LIB> FLAG1 n'est pas défini..." << std::endl;
#endif

#ifdef FLAG2
    std::cout << "LIB> FLAG2 est défini, yay !" << std::endl;
#else
    std::cout << "LIB> FLAG2 n'est pas défini..." << std::endl;
#endif
}
