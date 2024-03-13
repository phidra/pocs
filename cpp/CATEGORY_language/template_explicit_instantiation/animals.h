#pragma once

#include <string>

struct Cat {
    inline std::string as_human_string() const { return "CAT"; }
};

struct Dog {
    inline std::string as_human_string() const { return "DOG"; }
};

struct Bird {
    inline std::string as_human_string() const { return "BIRD"; }
};
