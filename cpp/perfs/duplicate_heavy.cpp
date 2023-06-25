#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

// EXPECTED ON FLAMEGRAPH = all time passed in "heavy", sadly (it would have been better to distinguish both calls)

using namespace std;

void heavy_function(vector<int>::size_type iterations);

int main(int argc, char* argv[])
{
    auto time_point_1 = chrono::steady_clock::now();
    heavy_function(6*1000*1000);
    auto time_point_2 = chrono::steady_clock::now();
    cout << "call 1 took  : " << chrono::duration_cast<chrono::milliseconds>(time_point_2-time_point_1).count() << " ms" << endl;
    heavy_function(12*1000*1000);
    auto time_point_3 = chrono::steady_clock::now();
    cout << "call 2 took  : " << chrono::duration_cast<chrono::milliseconds>(time_point_3-time_point_2).count() << " ms" << endl;
}

void heavy_function(vector<int>::size_type iterations)
{
    vector<int> v;
    for (vector<int>::size_type i = 0; i < iterations; ++i)
    {
        v.push_back(i*i + 42 * (cos(static_cast<float>(i)) / sin(static_cast<float>(2*i + 1)) + sqrt(static_cast<float>(3+i))));
    }
}

