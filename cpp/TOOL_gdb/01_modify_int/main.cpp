#include <iostream>

void myfunction(int age) {
    std::cout << "Captain's age is " << age << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    std::cout << R"DELIM(
This POC shows how to modify an int with gdb.
The C++ code is simple : display an int.
The POC's interest is in gdb commands : setting value of 'age' to modify its display.
)DELIM";

    // calling myfunction several times, with the same args :
    int nb_calls = 3;
    for (int i = 0; i < nb_calls; ++i) {
        std::cout << "BEFORE call #" << i << std::endl;
        myfunction(42);
        std::cout << "AFTER  call #" << i << std::endl;
        std::cout << std::endl;
    }
    return 0;
}
