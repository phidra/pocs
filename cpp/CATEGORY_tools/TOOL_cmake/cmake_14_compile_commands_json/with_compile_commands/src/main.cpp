#include <iostream>

// WITH compile_commands.json, this include is properly found by ALE (thus no error is displayed) :
#include "lib.h"

int main(void) {
    int x = 42;
    int y = 100;
    int sum = adder(x, y);
    std::cout << "Sum of " << x << " and " << y << " is " << sum << std::endl;
    return 0;
}
