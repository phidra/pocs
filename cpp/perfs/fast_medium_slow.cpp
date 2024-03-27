#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

// EXPECTED ON FLAMEGRAPH = most of the time in 'heavy', moderate time in 'medium', almost no time in 'light'

using namespace std;

void heavy_function();
void medium_function();
void light_function();

int main(int argc, char* argv[]) {
    auto time_point_1 = chrono::steady_clock::now();
    light_function();
    auto time_point_2 = chrono::steady_clock::now();
    cout << "light took  : " << chrono::duration_cast<chrono::milliseconds>(time_point_2 - time_point_1).count()
         << " ms" << endl;
    medium_function();
    auto time_point_3 = chrono::steady_clock::now();
    cout << "medium took : " << chrono::duration_cast<chrono::milliseconds>(time_point_3 - time_point_2).count()
         << " ms" << endl;
    heavy_function();
    auto time_point_4 = chrono::steady_clock::now();
    cout << "heavy took  : " << chrono::duration_cast<chrono::milliseconds>(time_point_4 - time_point_3).count()
         << " ms" << endl;
}

void compute(vector<int>::size_type iterations) {
    vector<int> v;
    for (vector<int>::size_type i = 0; i < iterations; ++i) {
        v.push_back(i * i + 42 * (cos(static_cast<float>(i)) / sin(static_cast<float>(2 * i + 1)) +
                                  sqrt(static_cast<float>(3 + i))));
    }
}

void heavy_function() {
    compute(10 * 1000 * 1000);
}
void medium_function() {
    compute(1000 * 1000);
}
void light_function() {
    compute(100 * 1000);
}
