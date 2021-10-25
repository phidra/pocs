#include <iostream>

template <typename TInspector, typename T1, typename T2>
TInspector wrap1(T1 x1, T2 x2) {
    std::cout << "[WRAPPED 1] ";
    return TInspector{x1, x2};
}
