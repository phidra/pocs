#include <iomanip>
#include <iostream>

using namespace std;

constexpr int const value_type_description_width = 25;

// this macro determines the type of x : lvalue or rvalue
// (a macro is used instead of a template to avoid the pitfalls of template deduction)
#define CHECK_VALUE_TYPE(type, description)                                         \
    cout << left;                                                                   \
    cout << description;                                                            \
    if (!is_reference<type>::value)                                                 \
        cout << setw(value_type_description_width) << " is not a reference ";       \
    else if (is_lvalue_reference<type>::value && !is_rvalue_reference<type>::value) \
        cout << setw(value_type_description_width) << " is a ref to an LVALUE  ";   \
    else if (!is_lvalue_reference<type>::value && is_rvalue_reference<type>::value) \
        cout << setw(value_type_description_width) << " is a ref to an RVALUE  ";   \
    else                                                                            \
        cout << setw(value_type_description_width) << " WTF ?! is a both a ref to an RVALUE and a LVALUE ?!  ";

template <typename T>
void f(T t) {
    CHECK_VALUE_TYPE(T, "T");
    cout << endl;
    CHECK_VALUE_TYPE(decltype(t), "t");
    cout << endl;

    T& m = t;  // m as in "mutable"
    ++m;

    CHECK_VALUE_TYPE(decltype(m), "m");
    cout << endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
The purpose of this POC is to illustrate another situation where the reference-collapsing may occur.

Reference-collapsing happens when we take a reference of a reference : in this POC, we use templates to achieve this.

We EXCPLICITLY instanciate a template-function f with a rvalue-reference (T=int&&).
Inside the template-function, we take a T&.
As T is int&& , T& is int&& &  -> the POC show that it is collapsed in int&

(of course, this is consistent with the reference-collapsing rules : the & is stronger than the &&)

)DELIM";

    f<int&&>(42);

    return 0;
}
