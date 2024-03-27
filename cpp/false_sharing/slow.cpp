#include <iostream>
#include <sstream>
#include <thread>

void compute(unsigned long long const size, unsigned long long& result) {
    // SLOW : directly working on the result, thus false sharing ensues
    for (int i = 0; i < size; ++i) {
        (i % 2 == 0) ? result += 3 : result -= 2;
    }
}

void check(unsigned long long const size, unsigned long long const result) {
    unsigned long long const expected_even = size / 2;     // when input size is even
    unsigned long long const expected_odd = size / 2 + 3;  // when input size is odd
    if (result != expected_even && result != expected_odd) {
        std::ostringstream oss;
        oss << "wrong result : " << result;
        throw std::logic_error(oss.str());
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << std::endl;
        std::cerr << "USAGE: " << argv[0] << "  SIZE" << std::endl;
        std::cout << std::endl;
        return 1;
    }

    unsigned long long const size = std::stoull(argv[1]);
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
