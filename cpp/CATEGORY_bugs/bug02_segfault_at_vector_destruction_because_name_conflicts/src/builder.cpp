#include "builder.h"

// from this translation unit's point of view, my::Pouet.hum is a vector<int>

my::Pouet my::build() {
    my::Pouet p;
    p.hum.push_back(3);
    p.hum.push_back(4);
    p.hum.push_back(5);
    p.hum.push_back(6);
    p.hum.push_back(7);
    return p;
}
