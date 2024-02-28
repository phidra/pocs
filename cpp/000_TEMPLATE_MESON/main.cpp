#include <iostream>

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = TODO TODO TODO

- TODO
- TODO
- TODO
- TODO

TODO

)DELIM";
    cout << endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    cout << "TODO" << endl;
    cout << "TODO" << endl;
    cout << "TODO" << endl;
}
