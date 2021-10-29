#include <iostream>

using namespace std;

void explicit_creation_of_references() {
    cout << "======= Those 4 steps test the DIRECT creation of references :" << endl;

    cout << "=== STEP 1 = binding a non-const reference to a non-temporary int :" << endl;
    int a1 = 42;
    cout << "a1 = " << a1 << endl;
    int& ref1 =
        a1;      // non-const reference = "mutating" reference (a reference that allow to mutate the referred object)
    ref1 = 100;  // mutating a1, through the ref
    cout << "a1 = " << a1 << " (expected value is 100)" << endl;
    cout << endl;

    cout << "=== STEP 2 = binding a const-reference to a non-temporary int :" << endl;
    int a2 = 42;
    int const& ref2 = a2;  // const reference = "non-mutating" reference
    ++a2;                  // directly mutating the original int
    cout << "ref2 = " << ref2 << " (expected value is 43)" << endl;
    cout << endl;

    cout << "=== STEP 3 = binding a const-reference to a temporary int :" << endl;
    int const& ref3 = 42;  // const reference to a TEMPORARY
    cout << "ref3 = " << ref3 << " (expected value is 42)" << endl;
    cout << endl;

    cout << "=== STEP 4 = binding a non-const reference to a temporary int (IMPOSSIBLE) :" << endl;
    // int& ref4 = 42;  // NON-CONST reference to a TEMPORARY
    // ^ the above line, when uncommented, does NOT compile, with this error :
    // error: cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’
    // thus, we also comment the following line :
    // cout << "ref4 = " << ref4 << " (expected value is 42)" << endl;
    cout << "We are not allowed to do that..." << endl;
    cout << endl;
}

void function_that_expects_a_CONST_lvalue_reference(int const& arg) {
    cout << "Passed arg = " << arg << endl;
    cout << "(we do not mutate the arg, as it's passed as a const-ref)" << endl;
}

void function_that_expects_a_NONCONST_lvalue_reference(int& arg) {
    cout << "Passed arg before mutating = " << arg << endl;
    ++arg;
    cout << "Passed arg after mutating= " << arg << endl;
}

void implicit_creation_of_references_through_argument_passing() {
    cout << "======= Those 4 steps test the same situations, but the reference are created INDIRECTLY, when values are "
            "passed to a function :"
         << endl;
    cout << "More precisely, calling the functions creates its arguments (on the stack)" << endl;
    cout << "If one of these arguments is a reference, calling the function will create a reference, and binds it to "
            "something"
         << endl;

    cout << "=== STEP 1 = the function-call binds a non-const reference to a non-temporary int :" << endl;
    int a1 = 42;
    cout << "BEFORE calling the function, a1 = " << a1 << endl;
    function_that_expects_a_NONCONST_lvalue_reference(a1);
    cout << "AFTER  calling the function, a1 = " << a1 << " (expected value is 43)" << endl;
    cout << endl;

    cout << "=== STEP 2 = the function-call binds a const-reference to a non-temporary int :" << endl;
    int a2 = 42;
    function_that_expects_a_CONST_lvalue_reference(a2);
    cout << endl;

    cout << "=== STEP 3 = the function-call binds a const-reference to a temporary int :" << endl;
    function_that_expects_a_CONST_lvalue_reference(42);
    cout << endl;

    cout << "=== STEP 4 = the function-call binds a non-const reference to a temporary int (IMPOSSIBLE) :" << endl;
    /* function_that_expects_a_NONCONST_lvalue_reference(42); */
    // ^ the above line, when uncommented, does NOT compile, with this error :
    // error: cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’
    cout << "We are not allowed to do that..." << endl;
    cout << endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
The purpose of the POC is to illustrate that :
    1. it's (curiously !) possible to bind a const reference to an rvalue
    2. it's not possible to bind a non-const reference to an rvalue

Moreover, in the POC, the illustration is made by :
    - DIRECTLY creating a reference
    - INDIRECTLY creating a reference, through passing an argument to functions expecting a const or non-const reference

About the point 1. :
    There is a GOTW = https://herbsutter.com/2008/01/01/gotw-88-a-candidate-for-the-most-important-const/

    Normally, a temporary object lasts only until the end of the full expression in which it appears.
    However, C++ deliberately specifies that binding a temporary object to a reference to const on the
    stack lengthens the lifetime of the temporary to the lifetime of the reference itself

    Note this only applies to stack-based references. It doesn’t work for references that are members of objects.

About the point 2. , it's quite logical :
    Binding non-const reference to something means "we will mutate that something".
    If 'something' is a temporary, it will be deleted soon.
    So there is no point in mutating the variable, as it will be deleted soon.

NOTE : in the cout/comments of the POC, I use "temporary" to speak about an rvalue.

)DELIM";

    explicit_creation_of_references();
    implicit_creation_of_references_through_argument_passing();
    return 0;
}
