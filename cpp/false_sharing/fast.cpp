#include <iostream>
#include <sstream>
#include <thread>

#define ALIGNMENT 64

void compute(const unsigned long long size, unsigned long long& result)
{
    // COULD BE SLOW : directly working on the result, but alignment constraint ensures to avoid false sharing.
    for (int i = 0; i < size; ++i)
    {
        (i % 2 == 0) ? result += 3 : result -= 2;
    }
}

void check(const unsigned long long size, const unsigned long long result)
{
    const unsigned long long expected_even = size / 2;  // when input size is even
    const unsigned long long expected_odd = size / 2 + 3;  // when input size is odd
    if (result != expected_even && result != expected_odd)
    {
        std::ostringstream oss;
        oss << "wrong result : " << result;
        throw std::logic_error(oss.str());
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << std::endl;
        std::cerr << "USAGE: " << argv[0] << "  SIZE" << std::endl;
        std::cout << std::endl;
        return 1;
    }

    const unsigned long long size = std::stoull(argv[1]);
    unsigned long long result __attribute__ ((aligned (ALIGNMENT))) = 0;
    unsigned long long result2 __attribute__ ((aligned (ALIGNMENT))) = 0;
    auto th = std::thread(compute, size, std::ref(result));
    compute(size, result2);
    th.join();
    check(size, result);
    check(size, result2);

    /* std::cout << "res 1 = " << result << std::endl; */
    /* std::cout << "res 2 = " << result << std::endl; */
    /* std::cout << std::endl; */

    return 0;
}
