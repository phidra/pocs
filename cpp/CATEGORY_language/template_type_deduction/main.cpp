#include <iomanip>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>

using namespace std;

constexpr int const input_type_description_width = 10;
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
void pass_by_value(T t, string input_type_description) {
    cout << "when given : " << setw(input_type_description_width) << input_type_description << " | ";
    CHECK_VALUE_TYPE(T, "T");
    CHECK_VALUE_TYPE(decltype(t), "t");
    cout << endl;
}

template <typename T>
// note : a const-ref is used so that we are allowed to pass an rvalue (otherwise, we can't bind a non-const ref to a
// rvalue)
void pass_by_lref(T const& t, string input_type_description) {
    cout << "when given : " << setw(input_type_description_width) << input_type_description << " | ";
    CHECK_VALUE_TYPE(T, "T");
    CHECK_VALUE_TYPE(decltype(t), "t");
    cout << endl;
}

template <typename T>
void pass_by_rref(T&& t, string input_type_description) {
    cout << "when given : " << setw(input_type_description_width) << input_type_description << " | ";
    CHECK_VALUE_TYPE(T, "T");
    CHECK_VALUE_TYPE(decltype(t), "t");
    cout << endl;
}

void poc_with_argument_deduction() {
    int i = 42;
    int& lvalue_ref = i;

    cout << "=== TYPE-DEDUCING CASE 1 = USING pass_by_value(T t) :" << endl;
    pass_by_value(i, "i");
    pass_by_value(lvalue_ref, "lvalue_ref");
    pass_by_value(111, "literal");
    cout << endl;

    cout << "=== TYPE-DEDUCING CASE 2 = USING pass_by_lref(T const& t) :" << endl;
    pass_by_lref(i, "i");
    pass_by_lref(lvalue_ref, "lvalue_ref");
    pass_by_lref(222, "literal");
    cout << endl;

    cout << "=== TYPE-DEDUCING CASE 3 = USING pass_by_rref(T&& t) :" << endl;
    pass_by_rref(i, "i");
    pass_by_rref(lvalue_ref, "lvalue_ref");
    pass_by_rref(333, "literal");
    cout << endl;
}

void poc_with_explicit_instanciation() {
    int i = 42;
    int& lvalue_ref = i;

    cout << "=== EXPLICIT-INSTANCIATION CASE 1 = USING pass_by_value(T t) :" << endl;
    pass_by_value<int>(i, "i");
    pass_by_value<int&>(lvalue_ref, "lvalue_ref");
    pass_by_value<int&&>(111, "literal");
    cout << endl;

    cout << "=== EXPLICIT-INSTANCIATION CASE 2 = USING pass_by_lref(T const& t) :" << endl;
    pass_by_lref<int>(i, "i");
    pass_by_lref<int&>(lvalue_ref, "lvalue_ref");
    pass_by_lref<int const&&>(222, "literal");
    cout << endl;

    cout << "=== EXPLICIT-INSTANCIATION CASE 3 = USING pass_by_rref(T&& t) :" << endl;
    // this does not compile (unless we move(i)), beacause we cannot create T&& on a non-reference integer
    // pass_by_rref<int>(i, "i");
    pass_by_rref<int&>(lvalue_ref, "lvalue_ref");
    pass_by_rref<int&&>(333, "literal");
    cout << endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
The purpose of this POC is to better understand template-argument deduction.

TL;DR : type-deduction rules are not as intuitive as they may appear, one must be careful...

DOC = https://en.cppreference.com/w/cpp/language/template_argument_deduction
Link between this POC and the doc :
    P in the doc is decltype(t) in the POC
    A in the doc is T in the POC

The POC makes tests in various situation, that differ along 3 axis :
    AXIS 1 = explicit-instanciation : pass_by_lref<int>(...)) vs. implicit-deduction : pass_by_lref(...)
    AXIS 2 = the signature of the template-declaration :
        void pass_by_value(T t)
        void pass_by_lref(T const& t)
        void pass_by_rref(T&& t)
    AXIS 3 = for a given signature, the type of the variable passed as argument :
        int i = 42;
        int& lvalue_ref = i;
        pass_by_value(i);
        pass_by_value(lvalue_ref);
        pass_by_value(111);

For each test, it displays the value-type (not-a-reference, ref-to-an-lvalue, ref-to-an-rvalue) of the template-parameter T, and of the argument decltype(t).
As a reminder, the deduced value-type of T (e.g. int) may be different from the value-type of t (e.g. int const&)

LEARNED FROM THIS POC = better understanding of the deduction rules :
    - as a (counter-intuitive) base rule, the deduced type (T) is always the decayed type = non-const + non-reference type !
        thus 1, for this situation, T and t will be "int" (and NOT int&) :
            int& ref = i;
            pass_by_value(i);
        thus 2, for this situation, T will be "int" (and NOT int&), and t will be "int&" :
            int& ref = i;
            pass_by_lref(i);
        thus 3, for this situation, T will be "int" (and NOT int&& !), and t will be "int&", the rvalue-reference is lost :
            int i = 42;
            pass_by_lref(move(i));
    - some special rule apply for the so-called universal-reference (T&& t) :
        in this special case, the base-rule does NOT apply, instead :
            subcase1 : if the passed-parameter is an lvalue of type U, T is deduced as U&
            subcase2 : if the passed-parameter is an rvalue of type U, T is deduced as U
        thus, the type of "t" is :
            subcase1 : U& && = U&, due to the reference-collapsing
            subcase1 : U&& && = U&&, due to the reference-collapsing
        in conclusion :
            if the passed-parameter is an lvalue of type U, t is of type U&,
            if the passed-parameter is an rvalue of type U, t is of type U&&,
    - IMPORTANT NOTE : it is ONLY in type-deducing context that "T&& t" has this special-meaning of "universal-reference".
    - (in combination with std::forward, this special rule allow perfect forwarding aka wrapping a function call)
    - all these deduction rules are ignored with EXPLICIT template-instanciation
        but beware : reference-collapsing still applies, and if T is int&&, here, t will be a int& :   pass_by_lref(T const& t)
    - (untested) the same deduction rules apply for type-deducing with auto

USEFUL REFERENCES :
    https://en.cppreference.com/w/cpp/language/template_argument_deduction
    https://levelup.gitconnected.com/pitfalls-of-templates-type-deduction-in-c-272d66bc75aa
    https://zestedesavoir.com/tutoriels/474/la-deduction-de-type-en-c/
    https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c
       ^ good explanation of the special-rule

)DELIM";

    // AXE 1 = template argument deduction vs. explicit template-instanciation :

    cout << "==========================================================================================" << endl;
    cout << "Using (implicit) argument deduction :" << endl;
    cout << "==========================================================================================" << endl;
    poc_with_argument_deduction();
    cout << endl;

    cout << "==========================================================================================" << endl;
    cout << "Using explicit template-instanciation :" << endl;
    cout << "==========================================================================================" << endl;
    poc_with_explicit_instanciation();

    return 0;
}
