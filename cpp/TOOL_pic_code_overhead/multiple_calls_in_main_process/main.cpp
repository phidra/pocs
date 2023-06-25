#include <iostream>
#include <chrono>

constexpr int NB_ACCUMULATIONS = 500000000;

void __attribute__ ((noinline)) accumulate(int& accumulator, const int& value);
extern int global_var;

int main(int argc, char* argv[])
{
    int accumulator = 0;
    auto before = std::chrono::steady_clock::now();
    for (int i = 0; i < NB_ACCUMULATIONS; ++i)
    {
        accumulate(accumulator, 42);  // function-call in shared object
        global_var += 3*i;  // global variable use in shared object
    }
    auto after = std::chrono::steady_clock::now();
    std::cout << "Took : " << std::chrono::duration_cast<std::chrono::milliseconds>(after-before).count() << " ms" << std::endl;
    std::cout << (accumulator > 0 ? " " : "  ") << std::endl;  // prevent compiler to remove dead "accumulator"
    std::cout << (accumulator > 0 ? " " : "  ") << std::endl;  // prevent compiler to remove dead "global_var"
    return 0;
}
