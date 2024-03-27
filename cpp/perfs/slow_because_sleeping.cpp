#include <iostream>
#include <chrono>
#include <thread>

// FLAMEGRAPH :
// - I expected : all the time is passed in 'sleeping_function'
// - I got : almost no sample captured at all
// - maybe it's because perf captures user-space samples ? Nope, even as root, perf don't capture anything
// - my guess : when process is sleeping, there are no perf_events to record

using namespace std;

void sleeping_function();

int main(int argc, char* argv[]) {
    auto time_point_1 = chrono::steady_clock::now();
    sleeping_function();
    auto time_point_2 = chrono::steady_clock::now();
    cout << "took : " << chrono::duration_cast<chrono::milliseconds>(time_point_2 - time_point_1).count() << " ms"
         << endl;
}

void sleeping_function() {
    this_thread::sleep_for(chrono::milliseconds(1500));
}
