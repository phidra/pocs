#include <omp.h>

#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void f_linear(vector<int>::size_type);
void f_parallel(vector<int>::size_type);
int compute(int i) {
    return i * i * static_cast<int>(sqrt(static_cast<float>(i))) - 5555 * sin(static_cast<float>(i * i));
}

int main(int argc, char* argv[]) {
    vector<int>::size_type size = 10 * 1000 * 1000;

    cout << "running f_linear" << endl;
    auto time_point_1 = chrono::steady_clock::now();
    f_linear(size);
    auto time_point_2 = chrono::steady_clock::now();
    cout << "f_linear took : " << chrono::duration_cast<chrono::milliseconds>(time_point_2 - time_point_1).count()
         << " ms" << endl;

    cout << "running f_parallel" << endl;
    auto time_point_3 = chrono::steady_clock::now();
    f_parallel(size);
    auto time_point_4 = chrono::steady_clock::now();
    cout << "f_parallel took : " << chrono::duration_cast<chrono::milliseconds>(time_point_4 - time_point_3).count()
         << " ms" << endl;
}

void f_linear(vector<int>::size_type size) {
    vector<string> v(100, "");
    string aaa = string{"AAAAA"};
    string bbb = string{"BBBBB"};
    for (vector<int>::size_type i = 0; i < size; ++i) {
        v[i % 100] = ((compute(i) % 2) == 0) ? aaa : bbb;
        assert(v[i % 100] == aaa || v[i % 100] == bbb);
    }
}

void f_parallel(vector<int>::size_type size) {
    vector<string> v(100, "");
    string aaa = string{"AAAAA"};
    string bbb = string{"BBBBB"};
#pragma omp parallel for
    for (vector<int>::size_type i = 0; i < size; ++i) {
        v[i % 100] = ((compute(i) % 2) == 0) ? aaa : bbb;
        string thecopy = v[i % 100];
        if (thecopy != aaa && thecopy != bbb) {
            cout << "ERROR : thecopy = '" << thecopy << "'" << endl;
            throw logic_error("fail");
        }
    }
}
