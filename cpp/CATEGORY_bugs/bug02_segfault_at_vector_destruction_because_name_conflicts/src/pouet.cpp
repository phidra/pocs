#include "pouet.h"

// this translation unit has no real purpose, other than "proving" that the object code
// of ~Pouet is not defined here
// (if that was the case, the destruction of hum would call vector<int>::~vector )

int my::Pouet::dasize() const {
    return 42 + hum.size();
}
