#include <iomanip>
#include <iostream>
#include <vector>

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

void inspect([[maybe_unused]] vector<int>&& x) {
    cout << "the overload expecting Rvalue-reference was called" << endl;
}

void inspect([[maybe_unused]] vector<int> const& x) {
    cout << "the overload expecting Lvalue-reference was called" << endl;
}

void f(vector<int>&& items) {
    CHECK_VALUE_TYPE(decltype(items), "items from inside the function 'f'");
    cout << endl;

    // there is no std::move here :
    inspect(items);
    // ^ the main knowledge of this POC is that in this case, the Lvalue overload of 'inspect' is called

    // this is just to show that move/static_cast<T&&> are equivalent from 'inspect' point-of-view :
    inspect(static_cast<vector<int>&&>(items));

    // there IS a std::move here :
    inspect(move(items));
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
This POC investigates why std::move is needed, if a function just uses an rvalue reference that it got as input.

What the POC defines :

- an overload-set of two function 'inspect' that behave differently if arg is an Lvalue or an Rvalue
- a function 'f' that expects an rvalue (vector<int>&&) as input, and calls inspect with it, WITHOUT moving it
- the main juste builds a vector<int>, moves it into the call of 'f'
- the purpose is to check if the overload with Rvalue or Lvalue is called
- a macro CHECK_VALUE_TYPE is used to see if 'items' is an lvalue or ravlue inside 'f'

What happens :
- if 'f' doesn't use std::move, the lvalue overload of 'inspect(items)' is called...
- ... even if 'items' is an rvalue-reference !
- this is even more confusing when the CHECK_VALUE_TYPE macro sees 'items' as an Rvalue !
- of course, if f uses move(items) (or static_cast<vector<int>>&&(items)), the proper rvalue overload is called

This (excellent) article has a sentence on this topic :
    http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html
    But when used inside the function body, a parameter, whether lvalue reference or rvalue reference, is an lvalue itself: it has a name like any other variable.

But I'm confused : I would have expect that the macro returns 'LVALUE'...
My guess, is that decltype uses the passed parameter, and not the actual parameter.

Still, the rule-of-thumb remains : use std::move to pass a parameter by rvalue-ref (and don't use the object anymore).

INTERESTING ARTICLE = https://www.foonathan.net/2020/09/move-forward/

Quote 1 : "Inside the function, the rvalue arguments create rvalue reference parameters, which are themselves lvalues as they are named!"
Quote 2 : "However, now the returned rvalue reference [returned from std::forward] doesn’t have a name which makes it an rvalue!


)DELIM";

    vector<int> items{1, 3, 5, 7, 42};
    f(move(items));
    cout << endl;

    return 0;
}
