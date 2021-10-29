#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
This POC show the use of std::ostream_iterator to easily display a vector.

Seen in : https://h-deb.clg.qc.ca/Sujets/TrucsScouts/intro_std_function.html

Cf. https://en.cppreference.com/w/cpp/iterator/ostream_iterator
)DELIM";

    auto items = vector<int>{1, 3, 5, 7, 9};

    cout << "with explicit for-loop :" << endl;
    for (auto item : items) {
        cout << item << "\n";
    }
    cout << endl;

    cout << "with ostream_iterator :" << endl;
    copy(items.begin(), items.end(), ostream_iterator<int>(cout, "\n"));
    cout << endl;

    return 0;
}
