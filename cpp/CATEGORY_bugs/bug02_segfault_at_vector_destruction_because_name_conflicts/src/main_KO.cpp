#include <iostream>

// As my::Pouet uses the alias StopId, this alias will affect Pouet's definition, and indirectly cause segfault :
namespace my {
using StopId = std::string;
}

#include "builder.h"
#include "pouet.h"

// from now on, from this translation unit's point of view, my::Pouet.hum is a vector<string>.
// Thus, the important thing is :
//      my::build builds a Pouet (with a vector<int>) that is NOT THE SAME that the one seen in this main (with a
//      vector<string>)

using namespace std;

int main(void) {
    // my::build() creates a Pouet, where p.hum is a vector<int>
    // but from main_KO's point of view, p.hum is  a vector<string>
    // at the end of the scope, the destruction of Pouet causes the destruction of hum
    // vector<string>::~vector is called on an object that is a vector<int> --> segfault
    my::Pouet p = my::build();
    cout << "size = " << p.hum.size() << endl;

    return 0;
}
