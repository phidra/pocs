#include <omp.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

// basic case = parallelizing a heavy computation.
//
// evaluated on two computers (lscpu) :
//
// situation 1 = Intel(R) Core(TM) i3-2100 CPU @ 3.10GHz (4  CPUs :  2xcores / 2xthreads / 1xsocket)  -> ratio = 0.38
// f_linear took : 2330 ms
// f_parallel took : 890 ms
//
// situation 2 = Intel(R) Xeon(R) CPU E5-2650L v3 @ 1.80GHz (48 CPUs : 12xcores / 2xthreads / 2xsockets) -> ratio = 0.04
// f_linear took : 2622 ms
// f_parallel took : 96 ms
//
// how to compile :
//
//      g++ -std=c++11 -fopenmp main.cpp -o bin

using namespace std;

void f_linear(vector<int>&, vector<int>::size_type);
void f_parallel(vector<int>&, vector<int>::size_type);
int compute(int i) {
    return i * i * static_cast<int>(sqrt(static_cast<float>(i))) - 5555 * sin(static_cast<float>(i * i));
}

int main(int argc, char* argv[]) {
    vector<int>::size_type size = 50 * 1000 * 1000;
    vector<int> v_linear(size, 0);
    vector<int> v_parallel(size, 0);

    cout << "running f_linear" << endl;
    auto time_point_1 = chrono::steady_clock::now();
    f_linear(v_linear, size);
    auto time_point_2 = chrono::steady_clock::now();
    cout << "f_linear took : " << chrono::duration_cast<chrono::milliseconds>(time_point_2 - time_point_1).count()
         << " ms" << endl;

    cout << "running f_parallel" << endl;
    auto time_point_3 = chrono::steady_clock::now();
    f_parallel(v_parallel, size);
    auto time_point_4 = chrono::steady_clock::now();
    cout << "f_parallel took : " << chrono::duration_cast<chrono::milliseconds>(time_point_4 - time_point_3).count()
         << " ms" << endl;
}

void f_linear(vector<int>& v, vector<int>::size_type size) {
    for (vector<int>::size_type i = 0; i < size; ++i) {
        v[i] = compute(i);
    }
}

void f_parallel(vector<int>& v, vector<int>::size_type size) {
#pragma omp parallel for
    for (vector<int>::size_type i = 0; i < size; ++i) {
        v[i] = compute(i);
    }
}
