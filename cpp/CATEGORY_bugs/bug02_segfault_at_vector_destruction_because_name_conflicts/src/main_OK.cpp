#include <iostream>

#include "builder.h"
#include "pouet.h"

using namespace std;

int main(void) {

    my::Pouet p = my::build();
    cout << "size = " << p.hum.size() << endl;

    return 0;
}
