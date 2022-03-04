#include "operations.h"

#include <algorithm>

std::string special_reverse_string_cpp(std::string const& s) {
    auto temp = s;
    std::reverse(temp.begin(), temp.end());
    return std::string("BEFORE_") + temp + "_AFTER";
}

std::vector<int> compute_diff_cpp(std::vector<unsigned> const& input_vector) {
    std::vector<int> to_return;
    auto ite = input_vector.begin();
    unsigned previous = *ite;
    ++ite;

    for (; ite != input_vector.end(); ++ite) {
        unsigned current = *ite;
        to_return.push_back(static_cast<int>(current) - static_cast<int>(previous));
        previous = current;
    }
    return to_return;
}
