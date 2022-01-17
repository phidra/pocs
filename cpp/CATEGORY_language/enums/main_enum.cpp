#include <iostream>

using namespace std;

// This POC shows that unfortunately, enum can be initialized to a "forbidden" value.
// (i.e. an integer value, not declared in the enum)
// EDIT : but enum class cannot, see other poc \o/

int main(int argc, char** argv) {
    enum Direction { north, south, east, west };

    // creation before display :
    Direction dir = north;
    cout << "Creation before display   = " << dir << endl;
    cout << endl;

    // direct creation, on each possible values :
    cout << "Direct creation on north  = " << Direction(north) << endl;
    cout << "Direct creation on south  = " << Direction(south) << endl;
    cout << "Direct creation on east   = " << Direction(east) << endl;
    cout << "Direct creation on west   = " << Direction(west) << endl;
    cout << endl;

    // direct creation, on integers instead of possible values :
    cout << "Direct creation on int(0) = " << Direction(0) << endl;
    cout << "Direct creation on int(1) = " << Direction(1) << endl;
    cout << "Direct creation on int(2) = " << Direction(2) << endl;
    cout << "Direct creation on int(3) = " << Direction(3) << endl;
    cout << endl;

    // SADLY ALLOWED : direct creation, on forbidden integers :
    cout << "Direct creation on int(99) = " << Direction(99) << endl;
}
