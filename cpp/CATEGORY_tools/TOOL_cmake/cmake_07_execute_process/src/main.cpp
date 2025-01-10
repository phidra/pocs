#include <iostream>

#ifndef LS_PATH
#define LS_PATH "sadly, the PATH of ls is undefined :("
#endif

#ifndef LS_SIZE
#define LS_SIZE "sadly, the SIZE of ls is undefined :("
#endif

#ifndef LS_NB_LINES
#define LS_NB_LINES "sadly, the NB LINES of ls is undefined :("
#endif

int main(int argc, char* argv[]) {
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "The ls binary is :  " << LS_PATH << std::endl;
    std::cout << "The ls size   is :  " << LS_SIZE << std::endl;
    std::cout << "The ls nb lines  :  " << LS_NB_LINES << std::endl;
    std::cout << std::endl;
    return 0;
}
