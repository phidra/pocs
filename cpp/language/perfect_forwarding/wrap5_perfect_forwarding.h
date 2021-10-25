#include <iostream>

template <typename TInspector, typename T1, typename T2>
TInspector wrap5(T1&& x1, T2&& x2) {
    std::cout << "[WRAPPED 5 — perfect forwarding] ";
    return TInspector(std::forward<T1>(x1), std::forward<T2>(x2));
}
