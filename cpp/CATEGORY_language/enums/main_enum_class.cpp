#include <iostream>
using namespace std;

enum class Color {
    RED = 10,
    BLUE = 100,
    GREEN = 1000,
};

int main(int argc, char* argv[]) {
    Color c{Color::RED};

    // int i = Color::RED;  // won't compile : a color is NOT an integer
    // Color invalid2{42};  // won't compile : an integer is NOT a color
    Color invalid{static_cast<Color>(42)};  // sadly, this compiles :-(
    Color another_red{static_cast<Color>(0)};

    // a color is still an int under the hood, but it must be explicitly cast :
    cout << "color red  = " << static_cast<int>(c) << endl;
    cout << "color blue = " << static_cast<int>(Color::BLUE) << endl;
    cout << "color red  = " << static_cast<int>(another_red) << endl;
    cout << "color XXX  = " << static_cast<int>(invalid) << endl;
    return 0;
}
