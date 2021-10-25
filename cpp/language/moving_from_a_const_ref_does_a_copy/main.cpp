#include <iostream>

using namespace std;

struct X {
    // this class has NO constructor from a const X&&
    X() { cout << "default construction" << endl; }
    X(X const&) { cout << "copy-construction" << endl; }
    X(X&&) { cout << "move-construction" << endl; }
};

struct Y {
    // this class has a constructor from a const Y&& (the so-called wtf-construction)
    // even if it is not anymore useful (it's a const reference to a temporary...) than a copy, it is still called in
    // the POC
    Y() { cout << "default construction" << endl; }
    Y(Y const&) { cout << "copy-construction" << endl; }
    Y(Y&&) { cout << "move-construction" << endl; }
    Y(Y const&&) { cout << "wtf-construction !!!" << endl; }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
The purpose of this POC is to show that :
    1. moving from a const-reference is legal,
    2. in almost all the situations, it "fallbacks" to a copy-operation
    3. the reason WHY it fallbacks on the copy-operation is the (legitimate) absence of a const-rvalue-operation


The POC uses the construction of an object (X or Y) from a temporary (move(x) or move(y), to show that :
    - moving from a const object is allowed
    - indeed, std::move merely turns T into T&& -> if "T" is "X const", then move(x) is of type "X const&&" !
    - case X : when a move is made on a const object, the copy-constructor is called
        as X has no constructor that accepts a "X const&&", the better match is "X const&"
        this is why the copy-constructor is called
    - case Y : when a move is made on a const object, the wtf-constructor is called
        as Y has a constructor that accepts a "Y const&&", it is USED !
        I don't see any case where a cosntructor from a const reference to a temporary, is useful
        (apart maybe to explicitly delete it, in order to avoid accidental copy where a move is expected ?)

NOTE : clang-tidy has got a check on this :
    https://clang.llvm.org/extra/clang-tidy/checks/performance-move-const-arg.html
NOTE : see also this discussion :
    http://www.cplusplus.com/forum/general/232294/

)DELIM";

    // case X :
    X const x;
    X(move(x));  // will call copy-construction

    // case Y :
    Y const y;
    Y(move(y));  // will call wtf-construction !!

    return 0;
}
