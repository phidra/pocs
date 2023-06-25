#include <iostream>

using namespace std;

pair<char, int> compute(string in) {
    char first = in[0];
    int second = static_cast<int>(in[1]) - 4032;
    return {first, second};
}

int main(int argc, char* argv[]) {
    cout << "Coucou depuis le code C++" << endl;
    auto [left, right] = compute(argv[0]);
    cout << "left  = " << left << endl;
    cout << "right = " << right << endl;
    return 0;
}
