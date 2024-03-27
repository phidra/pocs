#ifndef ROUTING_KIT_MIN_MAX_H
#define ROUTING_KIT_MIN_MAX_H

#include <assert.h>

#include <vector>

namespace RoutingKit {

template <class T>
void min_to(T& x, const T& y) {
    if (y < x)
        x = y;
}

template <class T>
void max_to(T& x, const T& y) {
    if (y > x)
        x = y;
}

template <class T>
unsigned first_min_element_position_of(std::vector<T> const& v) {
    assert(!v.empty());
    unsigned pos = 0;
    for (unsigned i = 1; i < v.size(); ++i)
        if (v[i] < v[pos])
            pos = i;
    return pos;
}

template <class T>
const T& min_element_of(std::vector<T> const& v) {
    return v[first_min_element_position_of(v)];
}

template <class T>
const T& min_element_of(std::vector<T> const& v, const T& empty_value) {
    if (v.empty())
        return empty_value;
    else
        return v[first_min_element_position_of(v)];
}

template <class T>
unsigned first_max_element_position_of(std::vector<T> const& v) {
    assert(!v.empty());
    unsigned pos = 0;
    for (unsigned i = 1; i < v.size(); ++i)
        if (v[i] > v[pos])
            pos = i;
    return pos;
}

template <class T>
const T& max_element_of(std::vector<T> const& v) {
    return v[first_max_element_position_of(v)];
}

template <class T>
const T& max_element_of(std::vector<T> const& v, const T& empty_value) {
    if (v.empty())
        return empty_value;
    else
        return v[first_max_element_position_of(v)];
}

}  // namespace RoutingKit

#endif
