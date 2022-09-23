#include <iostream>

// WITHOUT compile_commands.json, this include is NOT found by ALE (thus an error is displayed) :
#include "lib.h"

int main(void) {
    int x = 42;
    int y = 100;
    int sum = adder(x, y);
    std::cout << "Sum of " << x << " and " << y << " is " << sum << std::endl;
    return 0;
}
