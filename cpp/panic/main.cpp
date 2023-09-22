#include <iostream>

#include "panic.h"

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = implémentation d'une fonction 'panic' pour réagir aux irrecoverable errors.

cf. https://buildingblock.ai/panic

)DELIM";
    cout << endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    cout << "BEFORE PANIC" << endl;
    panic::panic("pouet");
    cout << "AFTER PANIC" << endl;  // this line should never be executed
}
