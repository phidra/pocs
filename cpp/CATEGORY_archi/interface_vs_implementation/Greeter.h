#pragma once

#include <string>
#include <vector>

struct IGreeter {
    virtual void greet_many(std::vector<std::string> persons) const = 0;
    virtual ~IGreeter() = default;
};
