#include <iostream>

using namespace std;

// this macro determines the type of x : lvalue or rvalue
// (a macro is used instead of a template to avoid the pitfalls of template deduction)
#define CHECK_VALUE_TYPE(x, name)                                             \
    cout << name;                                                             \
    if (is_lvalue_reference<x>::value && !is_rvalue_reference<x>::value)      \
        cout << " is an LVALUE" << endl;                                      \
    else if (!is_lvalue_reference<x>::value && is_rvalue_reference<x>::value) \
        cout << " is an RVALUE" << endl;                                      \
    else                                                                      \
        cout << " WTF ?! is both an RVALUE and a LVALUE or neither ?!" << endl;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
The purpose of this POC is to illustrate the reference-collapsing rules.

Reference-collapsing happens when we take a reference of a reference : in this POC, we use typedefs to achieve this.

Results : basically, the & is stronger than the && :

    T& &&  turns into T&
    T&& &  turns into T&
    T& &   turns into T&
    T&& && turns into T&&  <-- it's the only case when we got a resulting &&

)DELIM";

    using l_reference = int&;
    using r_reference = int&&;
    using l_to_l = l_reference&;
    using r_to_l = l_reference&&;
    using l_to_r = r_reference&;
    using r_to_r = r_reference&&;

    CHECK_VALUE_TYPE(l_reference, "l_reference");
    CHECK_VALUE_TYPE(r_reference, "r_reference");
    CHECK_VALUE_TYPE(l_to_l, "l_to_l");
    CHECK_VALUE_TYPE(l_to_r, "l_to_r");
    CHECK_VALUE_TYPE(r_to_l, "r_to_l");
    CHECK_VALUE_TYPE(r_to_r, "r_to_r");

    return 0;
}
