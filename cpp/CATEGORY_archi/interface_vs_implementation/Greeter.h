#pragma once

#include <vector>
#include <string>

struct IGreeter {
    virtual void greet_many(std::vector<std::string> persons) const = 0;
    virtual ~IGreeter() = default;
};
