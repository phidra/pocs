#include <iostream>
#include <string>

void myfunction(std::string const& to_display) {
    std::cout << "Let me introduce you... " << to_display << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    std::cout << R"DELIM(
This POC shows how to modify a string with gdb.
The C++ code is simple : display a string.
The POC's interest is in gdb commands : calling 'to_display.assign' to modify the displayed string.
)DELIM";

    // calling myfunction several times, with the same args :
    int nb_calls = 3;
    for (int i = 0; i < nb_calls; ++i) {
        std::cout << "BEFORE call #" << i << std::endl;
        myfunction("pouet");
        std::cout << "AFTER  call #" << i << std::endl;
        std::cout << std::endl;
    }
    return 0;
}
