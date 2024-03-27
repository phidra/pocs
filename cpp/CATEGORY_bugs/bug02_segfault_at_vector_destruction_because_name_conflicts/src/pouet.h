#pragma once

#include <vector>

// this alias is in the global namespace
using StopId = int;

// the tricky thing is that : if a preexisting name "StopId" exists in the namespace my, it will
// be used instead of the global alias.

namespace my {

class Pouet {
   public:
    std::vector<StopId> hum;
    int dasize() const;
};

}  // namespace my
