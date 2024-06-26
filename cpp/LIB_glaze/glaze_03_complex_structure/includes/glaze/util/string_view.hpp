// Glaze Library
// For the license information refer to glaze.hpp

#pragma once

#include <string_view>

namespace glz {
using sv = std::string_view;

template <class T>
concept sv_convertible = std::convertible_to<std::decay_t<T>, std::string_view>;

struct comment final {
    std::string_view value{};

    constexpr comment(char const* str) noexcept : value(str) {}
    constexpr comment(char const* str, size_t n) noexcept : value(str, n) {}
};

constexpr comment operator""_c(char const* data, size_t n) noexcept {
    return {data, n};
}
}  // namespace glz
