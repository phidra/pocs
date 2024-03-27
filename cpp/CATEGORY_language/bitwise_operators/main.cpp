#include <bitset>
#include <iostream>
#include <sstream>

// This POC shows the effect of bitwise operators
// Particularly : how bits are "filled with zeros" with << and >>

using namespace std;

string as_string(unsigned long long value) {
    return bitset<8 * sizeof(int)>{value}.to_string();
}

int main(int argc, char* argv[]) {
    unsigned long long k = 45;
    cout << "bitwise shift ending with 1 :" << endl;
    cout << "k = " << k << " : " << as_string(k) << endl;
    cout << "k << 2 = " << as_string(k << 2) << endl;
    cout << "k >> 2 = " << as_string(k >> 2) << endl;
    cout << endl;

    unsigned long long i = 46;
    cout << "bitwise shift ending with 0 :" << endl;
    cout << "i = " << i << " : " << as_string(i) << endl;
    cout << "i << 2 = " << as_string(i << 2) << endl;
    cout << "i >> 2 = " << as_string(i >> 2) << endl;
    cout << endl;

    unsigned long long j = 5;
    cout << "bitwise XOR AND OR :" << endl;
    cout << "i = " << i << " : " << as_string(i) << endl;
    cout << "j =  " << j << " : " << as_string(j) << endl;
    cout << "i ^ j  = " << as_string(i ^ j) << endl;
    cout << "i & j  = " << as_string(i & j) << endl;
    cout << "i | j  = " << as_string(i | j) << endl;
    return 0;
}
