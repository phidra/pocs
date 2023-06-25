#include <iostream>
#include <chrono>
#include <thread>
#include "lib.h"


using namespace std;

constexpr int NB_ITERATIONS = 100*1000*1000;

int main(int argc, char* argv[])
{
    auto before = chrono::steady_clock::now();
    this_thread::sleep_for(chrono::milliseconds(1));

    for (int i = 0; i < NB_ITERATIONS; ++i)
    {
        pass();
    }

    auto after = chrono::steady_clock::now();
    cout << "Duration = " << chrono::duration_cast<chrono::milliseconds>(after - before).count() << " ms" << endl;
    return 0;
}
