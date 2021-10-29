#include <iostream>

template <typename TInspector, typename T1, typename T2>
TInspector wrap2(T1& x1, T2& x2) {
    std::cout << "[WRAPPED 2] ";
    return TInspector{x1, x2};
}
