#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// in this struct, we're only interested in the letter
struct Value {
    unsigned int number;
    char letter;
    float another_number;
};

int main() {
    cout << "Original vector :" << endl;
    vector<Value> mylist{
        {42, 'a', 0.5},
        {43, 'b', 0.7},
        {50, 'k', 0.3},
        {99, 'z', 0.1},
    };
    for (auto& x : mylist) {
        cout << x.number << " -> '" << x.letter << "' -> " << x.another_number << endl;
    }

    // python-equivalent of what I want :  mycomprehensionlist = [x.letter for x in mylist]
    vector<char> mycomprehensionlist(mylist.size());
    vector<Value>::size_type index = 0;
    generate(mycomprehensionlist.begin(), mycomprehensionlist.end(), [&]() { return mylist.at(index++).letter; });

    cout << endl << "Comprehension-list vector :" << endl;
    for (auto& x : mycomprehensionlist) {
        cout << "'" << x << "'" << endl;
    }

    return 0;
}
