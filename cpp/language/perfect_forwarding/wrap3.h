#include <iostream>

template <typename TInspector, typename T1, typename T2>
TInspector wrap3(T1 const& x1, T2 const& x2) {
    std::cout << "[WRAPPED 3] ";
    return TInspector{x1, x2};
}
