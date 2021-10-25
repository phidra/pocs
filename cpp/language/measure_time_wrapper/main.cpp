#include <iostream>
#include <iterator>
#include <vector>
#include <experimental/random>

#include "measure_time_wrapper.h"

using namespace std;

// we want to measure the execution time of this function :
vector<int> fill_vector(size_t size) {
    vector<int> to_return;
    for (size_t _ = 0; _ < size; ++_) {
        int random_number = experimental::randint(100, 999);
        to_return.push_back(random_number);
    }
    return to_return;
}

// this is another function (with 3 arguments instead of 1) that we want to measure :
vector<int> fill_vector_with_arbitrary_bounds(size_t size, int from, int to) {
    vector<int> to_return;
    for (size_t _ = 0; _ < size; ++_) {
        int random_number = experimental::randint(from, to);
        to_return.push_back(random_number);
    }
    return to_return;
}

void display(vector<int> const& items) {
    copy(items.begin(), items.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
This POC investigates how to make a wrapper that measures the time needed for a function to run.

Several versions exist :
    - (not ideal) measure_time_1 = only accepts a function void -> void , thus forcing lambda-capture
    - (not ideal) measure_time_2 = accepts ReturnType as template-parameter, but forces the explicit instanciation
    - (almost there) measure_time_3 = uses perfect-forwarding, but on a single arg
    - (correct) measure_time_4 = uses perfect-forwarding with variading-templates to enable arbitrary number of arguments

)DELIM";

    size_t LENGTH = 5;

    // it seems that initializing randint is time-consuming -> doing it so that tests are more identical :
    cout << "This is a random int : " << experimental::randint(100, 999) << endl;
    cout << endl;

    // measure_time_1 : the returned value must pre-exist :
    vector<int> items_1;
    int time_1 = measure_time_1([&items_1, LENGTH]() { items_1 = fill_vector(LENGTH); });
    display(items_1);
    cout << "Time 1 = " << time_1 << " µs" << endl;
    cout << endl;

    // measure_time_2 : the returned value can be created by the function :
    auto [items_2, time_2] = measure_time_2<vector<int>>([LENGTH]() { return fill_vector(LENGTH); });
    display(items_2);
    cout << "Time 2 = " << time_2 << " µs" << endl;
    cout << endl;

    // measure_time_3 : the template arguments may be deduced
    auto [items_3, time_3] = measure_time_3(fill_vector, LENGTH);
    display(items_3);
    cout << "Time 3 = " << time_3 << " µs" << endl;
    cout << endl;

    // measure_time_4 : the template arguments may be deduced
    auto [items_4, time_4] = measure_time_4(fill_vector_with_arbitrary_bounds, LENGTH, 10, 15);
    display(items_4);
    cout << "Time 4 = " << time_4 << " µs" << endl;
    cout << endl;

    return 0;
}
