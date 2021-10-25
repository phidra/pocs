#pragma once

#include <functional>
#include <chrono>
#include <type_traits>

// measure_time_1 = accepts a function without argument or returned value
// Designed to be used with a lambda that captures ALL that is relevant, including the "returned" value.
// Lambda-capturing probably doesn't preserve rvalue-reference (no perfect forwarding).
template <typename TimeGranularity = std::chrono::microseconds>
int measure_time_1(std::function<void(void)> const& f) {
    std::chrono::steady_clock::time_point before = std::chrono::steady_clock::now();
    f();
    std::chrono::steady_clock::time_point after = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<TimeGranularity>(after - before).count();
}

// measure_time_2 = accepts a function without argument, but with a returned value
// Apparently, compiler can't infer the returned-type from the passed lambda
// (and it must sadly be explicitly passed as a template-argument...)
// Moreover, lambda-capturing probably doesn't preserve rvalue-reference (no perfect forwarding).
template <typename ReturnType, typename TimeGranularity = std::chrono::microseconds>
std::tuple<ReturnType, int> measure_time_2(std::function<ReturnType(void)> const& f) {
    std::chrono::steady_clock::time_point before = std::chrono::steady_clock::now();
    ReturnType ret = f();
    std::chrono::steady_clock::time_point after = std::chrono::steady_clock::now();
    auto measured = std::chrono::duration_cast<TimeGranularity>(after - before).count();
    return std::make_tuple(ret, measured);
}

// measure_time_3 = callable is a template-parameter, its arg too
// uses perfect-forwarding (T&& + std::forward) to call the callable
// uses std::result_of_t to put the result in a tuple
// limitation = this wrapper can only be used on a callable with a single Arg
template <typename Callable, typename Arg, typename TimeGranularity = std::chrono::microseconds>
std::tuple<std::result_of_t<Callable && (Arg &&)>, int> measure_time_3(Callable f, Arg&& arg) {
    std::chrono::steady_clock::time_point before = std::chrono::steady_clock::now();
    auto ret = f(std::forward<Arg>(arg));
    std::chrono::steady_clock::time_point after = std::chrono::steady_clock::now();
    auto measured = std::chrono::duration_cast<TimeGranularity>(after - before).count();
    return std::make_tuple(move(ret), measured);
}

// measure_time_4 = same as measure_time_3, but uses variadic template to accept several args :
template <typename TimeGranularity = std::chrono::microseconds, typename Callable, typename... Args>
std::tuple<std::result_of_t<Callable && (Args && ...)>, int> measure_time_4(Callable f, Args&&... args) {
    std::chrono::steady_clock::time_point before = std::chrono::steady_clock::now();
    auto ret = f(std::forward<Args>(args)...);
    std::chrono::steady_clock::time_point after = std::chrono::steady_clock::now();
    auto measured = std::chrono::duration_cast<TimeGranularity>(after - before).count();
    return std::make_tuple(move(ret), measured);
}
