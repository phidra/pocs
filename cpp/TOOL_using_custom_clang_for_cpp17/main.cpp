#include <iostream>
#include <set>

using namespace std;

int main(int, char*[]) {
    set<int> mySet;
    auto [iter, inserted] = mySet.insert(42);
    cout << "Dit we insert an element ? " << boolalpha << inserted << endl;
    cout << "What is the inserted element ? " << *iter << endl;
    return 0;
}
