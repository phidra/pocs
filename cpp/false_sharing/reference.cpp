#include <iostream>
#include <sstream>
#include <thread>

void compute(const unsigned long long size, unsigned long long& result)
{
    // FAST : workon on a local accumulator, and only copying in the result at the end :
    unsigned long long acc = 0;
    for (int i = 0; i < size; ++i)
    {
        (i % 2 == 0) ? acc += 3 : acc -= 2;
    }
    result = acc;
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
    unsigned long long result = 0;
    unsigned long long result2 = 0;
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