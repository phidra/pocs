#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = les structures de base de std::chrono pour mesurer le temps :

      - duration    = elapsed period of time as a number of ticks, of a given unit
      - time_point  = number of elapsed ticks since a reference point (e.g. since epoch)
      - clock       = allows to get a time_point (defines a reference, and a tick-frequency)

Et comment s'en servir :

      - measure time = get a time_point BEFORE, a time_point AFTER, and computes duration

Plus une petite vérification = une "duration" est un nombre ENTIER de tickets (i.e. un tick n'est comptabilisé que
s'il a eu le temps de terminer).

)DELIM";
    cout << endl;
}

void howto_measure_elapsed_time() {
    // measure time :
    //      1. get a time_point BEFORE
    //      2. get a time_point AFTER
    //      3. compute duration, and cast it as a number of count in the desired unit
    auto before = chrono::steady_clock::now();
    this_thread::sleep_for(chrono::milliseconds(1000));
    auto after = chrono::steady_clock::now();
    cout << "We just slept for : " << chrono::duration_cast<chrono::milliseconds>(after - before).count() << endl;
    cout << endl;
}

void demonstrate_that_only_fully_elapsed_ticks_are_used() {
    // show that a duration is "just" an integer number of ticks
    // if the unit is large (here : seconds), the count will increase only
    // when a FULL second has passed.

    auto time_point_1 = chrono::steady_clock::now();
    cout << "BEGIN = sleeping for 900ms, a bit less than a second." << endl;
    this_thread::sleep_for(chrono::milliseconds(900));
    auto time_point_2 = chrono::steady_clock::now();

    auto count1 = chrono::duration_cast<chrono::seconds>(time_point_2 - time_point_1).count();
    cout << "The seconds 'count' is 0, because a whole second has not passed yet : " << count1 << endl;
    assert(count1 == 0);

    cout << "Sleeping for 200 ms more" << endl;
    this_thread::sleep_for(chrono::milliseconds(200));
    auto time_point_3 = chrono::steady_clock::now();
    auto count2 = chrono::duration_cast<chrono::seconds>(time_point_3 - time_point_1).count();
    cout << "The seconds 'count' is now 1, because NOW a whole second has passed : " << count2 << endl;
    assert(count2 == 1);
    cout << endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    howto_measure_elapsed_time();
    demonstrate_that_only_fully_elapsed_ticks_are_used();
}
