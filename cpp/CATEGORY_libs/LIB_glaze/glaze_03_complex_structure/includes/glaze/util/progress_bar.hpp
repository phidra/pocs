// Glaze Library
// For the license information refer to glaze.hpp

#pragma once

#include <cmath>
#include <iostream>
#include <string>

namespace glz {
struct progress_bar final {
    size_t width{};
    size_t completed{};
    size_t total{};
    double time_taken{};

    std::string string() const {
        std::string s{};

        const size_t one = 1;
        auto const one_or_total = (std::max)(total, one);
        auto const one_or_completed = (std::min)(completed, one_or_total);
        auto const progress = static_cast<double>(one_or_completed) / one_or_total;
        auto const percentage = static_cast<size_t>(std::round(progress * 100));

        if (width > 2) {
            auto const len = width - 2;
            auto const filled = static_cast<size_t>(std::round(progress * len));

            s += "[";

            for (size_t i = 0; i < filled; ++i) {
                s += '=';
            }

            for (size_t i = 0; i < len - filled; ++i) {
                s += '-';
            }

            s += "]";
        }

        auto const eta_s = static_cast<size_t>(
            std::round(((one_or_total - one_or_completed) * time_taken) / (std::max)(one_or_completed, one)));
        auto const minutes = eta_s / 60;
        auto const seconds = eta_s - minutes * 60;
        s += " " + std::to_string(std::lround(percentage)) + "%";
        s += " | ETA: " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
        s += " | " + std::to_string(one_or_completed) + "/" + std::to_string(one_or_total);

        // TODO: use std::format when available
        /*fmt::format_to(std::back_inserter(s), FMT_COMPILE(" {}% | ETA: {}m {}s | {}/{}"), //
                       std::round(percentage), //
                       minutes, //
                       seconds, //
                       one_or_completed,                                                   //
                       one_or_total);*/
        return s;
    }
};

inline std::ostream& operator<<(std::ostream& o, progress_bar const& bar) {
    o << bar.string();
    return o;
}
}  // namespace glz
