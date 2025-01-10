#include "name_modifier.h"

#include <algorithm>
#include <cctype>
#include <string>

#include "add_exclamation.h"

std::string modify_name(std::string name) {
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::toupper(c); });
    return add_exclamation(name);
}
