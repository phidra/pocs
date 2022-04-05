#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
This checks the contiguity of vector<int16_t>
)DELIM";

    vector<int16_t> v16 = {1, 2, 3, 4, 5};
    vector<int32_t> v32 = {1, 2, 3, 4, 5};

    cout << "DIFF 16 = " << reinterpret_cast<long long>(&(v16[1])) - reinterpret_cast<long long>(&(v16[0])) << endl;
    cout << "DIFF 32 = " << reinterpret_cast<long long>(&(v32[1])) - reinterpret_cast<long long>(&(v32[0])) << endl;

    return 0;
}
