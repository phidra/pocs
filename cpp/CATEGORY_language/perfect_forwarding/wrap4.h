#include <iostream>

template <typename TInspector, typename T1, typename T2>
TInspector wrap4(T1 const& x1, T2& x2) {
    std::cout << "[WRAPPED 4 — const& &] ";
    return TInspector{x1, x2};
}

template <typename TInspector, typename T1, typename T2>
TInspector wrap4(T1& x1, T2& x2) {
    std::cout << "[WRAPPED 4 — & &] ";
    return TInspector{x1, x2};
}

template <typename TInspector, typename T1, typename T2>
TInspector wrap4(T1&& x1, T2& x2) {
    std::cout << "[WRAPPED 4 — && &] ";
    return TInspector{std::move(x1), x2};
}

template <typename TInspector, typename T1, typename T2>
TInspector wrap4(T1 const& x1, T2 const& x2) {
    std::cout << "[WRAPPED 4 — const& const&] ";
    return TInspector{x1, x2};
}

template <typename TInspector, typename T1, typename T2>
TInspector wrap4(T1& x1, T2 const& x2) {
    std::cout << "[WRAPPED 4 — & const&] ";
    return TInspector{x1, x2};
}

template <typename TInspector, typename T1, typename T2>
TInspector wrap4(T1&& x1, T2 const& x2) {
    std::cout << "[WRAPPED 4 — && const&] ";
    return TInspector{std::move(x1), x2};
}

template <typename TInspector, typename T1, typename T2>
TInspector wrap4(T1 const& x1, T2&& x2) {
    std::cout << "[WRAPPED 4 — const& &&] ";
    return TInspector{x1, std::move(x2)};
}

template <typename TInspector, typename T1, typename T2>
TInspector wrap4(T1& x1, T2&& x2) {
    std::cout << "[WRAPPED 4 — & &&] ";
    return TInspector{x1, std::move(x2)};
}

template <typename TInspector, typename T1, typename T2>
TInspector wrap4(T1&& x1, T2&& x2) {
    std::cout << "[WRAPPED 4 — && &&] ";
    return TInspector{std::move(x1), std::move(x2)};
}
