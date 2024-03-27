#pragma once

#include <string>
#include <vector>

#include "Greeter.h"

struct GreeterImplThreaded : public IGreeter {
    void greet_many(std::vector<std::string> persons) const;
    virtual ~GreeterImplThreaded() = default;
};
