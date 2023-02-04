#pragma once

#include <vector>
#include <string>

#include "Greeter.h"

struct GreeterImplSimple : public IGreeter {
    void greet_many(std::vector<std::string> persons) const;
    virtual ~GreeterImplSimple() = default;
};
