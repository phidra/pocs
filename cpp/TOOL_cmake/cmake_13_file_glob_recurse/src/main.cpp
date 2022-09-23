#include <iostream>

#include "modules/module1.h"
#include "modules/submodules/module2.h"
#include "modules/submodules/deeper/module3.h"

using namespace std;

int main(void) {
    f1();
    f2();
    f3();
    return 0;
}
