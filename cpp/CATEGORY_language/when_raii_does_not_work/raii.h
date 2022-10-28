#include <iostream>

struct RAII {
    RAII() {
        std::cout << "\n\n";
        std::cout << "LOCKING\n" << std::flush;
    }
    ~RAII() {
        std::cout << "UNLOCKING   <---------- this is the desired print, that indicates that RAII worked\n"
                  << std::flush;
    }
};
