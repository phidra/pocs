#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

// EXPECTED ON FLAMEGRAPH = all the time is passed in 'heavy_function'

using namespace std;

void heavy_function();

int main(int argc, char* argv[]) {
    auto time_point_1 = chrono::steady_clock::now();
    heavy_function();
    auto time_point_2 = chrono::steady_clock::now();
    cout << "took : " << chrono::duration_cast<chrono::milliseconds>(time_point_2 - time_point_1).count() << " ms"
         << endl;
}

void heavy_function() {
    cout << "begin slow" << endl;
    vector<int> v;
    vector<int>::size_type MAX = 10 * 1000 * 1000;
    for (vector<int>::size_type i = 0; i < MAX; ++i) {
        v.push_back(i * i + 42 * (cos(static_cast<float>(i)) / sin(static_cast<float>(2 * i + 1)) +
                                  sqrt(static_cast<float>(3 + i))));
    }
    cout << "end slow" << endl;
}
