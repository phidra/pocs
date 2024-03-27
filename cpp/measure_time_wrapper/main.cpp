#include <cstdlib>
#include <ctime>
#include <experimental/random>
#include <iostream>
#include <iterator>
#include <vector>

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

void compute_without_return_type(size_t size) {
    int total = 0;
    for (size_t _ = 0; _ < size; ++_) {
        int random_number = experimental::randint(100, 999);
        total += random_number;
    }
    cout << "Total is " << total << endl;
}

int compute_with_default_args(size_t size, int lower_bound = 10, int upper_bound = 20) {
    int total = 0;
    for (size_t _ = 0; _ < size; ++_) {
        int random_number = experimental::randint(lower_bound, upper_bound);
        total += random_number;
    }
    return total;
}

template <typename TNumber>
TNumber templated_computed(size_t size, TNumber initial_value) {
    TNumber total = initial_value;
    for (size_t _ = 0; _ < size; ++_) {
        TNumber random_number = static_cast<TNumber>(rand()) / static_cast<TNumber>(RAND_MAX);
        total += random_number;
    }
    return total;
}

struct MyObject {
    MyObject(int initial_value_) : initial_value{initial_value_} {}
    int compute(size_t size) const {
        int total = this->initial_value;
        for (size_t _ = 0; _ < size; ++_) {
            int random_number = experimental::randint(42, 99);
            total += random_number;
        }
        return total;
    }
    int initial_value;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    srand(static_cast<unsigned>(time(0)));
    cout << R"DELIM(
This POC investigates how to make a wrapper that measures the time needed for a function to run.

Several versions exist :
    - (not ideal) measure_time_1 = only accepts a function void -> void , thus forcing lambda-capture
    - (not ideal) measure_time_2 = accepts ReturnType as template-parameter, but forces the explicit instanciation
    - (almost there) measure_time_3 = uses perfect-forwarding, but on a single arg
    - (correct) measure_time_4 = uses perfect-forwarding with variading-templates to enable arbitrary number of arguments

Even with the correct version, calling the wrapper is different from directly calling the function :
    - wrapper cannot be used on a function returning void
    - wrapper cannot be use on a function with default-argument unless it pass them
    - wrapper cannot deduce the template-arguments if the wrapped function is a template-function
    - wrapper cannot directly use a member-function, unless using std::bind

All those limitations are also illustrated below. Some of these might probably be avoided with additional work.
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

    // Below, limitations of measure_time_4 are illustrated. They are commented out :
    // LIMITATION 1 = current version of wrapper cannot be used on a function returning void :
    /* auto [_, limitation_time_1] = measure_time_4(compute_without_return_type, LENGTH); */

    // LIMITATION 2 = wrapper cannot use the wrapped function default arguments :
    cout << "When called directly, function can be called without passing the optional arguments : "
         << compute_with_default_args(LENGTH) << endl;
    auto [limitation_res_2, limitation_time_2] = measure_time_4(compute_with_default_args, LENGTH, 1000, 1001);
    cout << "When called through wrapper, function can be called WITH passing the optional arguments : "
         << limitation_res_2 << "  (took " << limitation_time_2 << " µs)" << endl;
    // but when we try to use the default args, BOOM ! This doesn't compile :
    /* auto [limitation_res_2_bis, limitation_time_2_bis] = measure_time_4(compute_with_default_args, LENGTH); */
    cout << endl;

    // LIMITATION 3 = wrapper cannot deduce the parameters of the arguments of a template-function :
    cout << "When called directly, template-function can deduce the arguments (float) : "
         << templated_computed(LENGTH, 4.5) << endl;
    cout << "When called directly, template-function can deduce the arguments (int)   : "
         << templated_computed(LENGTH, 99) << endl;
    auto [limitation_res_3, limitation_time_3] = measure_time_4(templated_computed<float>, LENGTH, 4.5);
    auto [limitation_res_3_bis, limitation_time_3_bis] = measure_time_4(templated_computed<int>, LENGTH, 99);
    cout << "When called through wrapper, template-arguments must be explicit (float) : " << limitation_res_3
         << "  (took " << limitation_time_3 << " µs)" << endl;
    cout << "When called through wrapper, template-arguments must be explicit (int) : " << limitation_res_3_bis
         << "  (took " << limitation_time_3_bis << " µs)" << endl;
    // but when we want the wrapped-function to deduce the args, BOOM ! This doesn't compile :
    /* auto [limitation_res_3_ter, limitation_time_3_ter] = measure_time_4(templated_computed, LENGTH, 4.5); */

    // LIMITATION 4 = wrapper cannot directly use on a member-function :
    MyObject x{20000};
    cout << "When called directly, we can use the member-function : " << x.compute(LENGTH) << endl;
    auto [limitation_res_4, limitation_time_4] = measure_time_4(bind(&MyObject::compute, &x, LENGTH));
    cout << "When called through wrapper, we must use std::bind (and also : probably lose the rvalue) : "
         << limitation_res_4 << "  (took " << limitation_time_4 << " µs)" << endl;
    // trying to direcly call the method doesn't seem to work, even when passing 'this' :
    /* auto [limitation_res_4_bis, limitation_time_4_bis] = measure_time_4(&MyObject::compute, &x, LENGTH); */
    /* auto [limitation_res_4_ter, limitation_time_4_ter] = measure_time_4(MyObject::compute, &x, LENGTH); */

    return 0;
}
