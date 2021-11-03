#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>

using namespace std;

// this class identifies with what it has been constructed : T const&  vs.  T&&
struct Inspect {
    Inspect([[maybe_unused]] vector<int>&& x) { cout << "the ctor expecting Rvalue-reference was called" << endl; }

    Inspect([[maybe_unused]] vector<int> const& x) { cout << "the ctor expecting Lvalue-reference was called" << endl; }
};

// INTENTION 1 = in the following cases, the intention is to forward the argument in a template context :

// INTENTION 1 CORRECT = this wrapper uses std::forward, as is expected in perfect-forwarding :
template <typename Callable, typename Arg>
void wrap_with_forward(Arg&& arg) {
    Callable{forward<Arg>(arg)};
}

// INTENTION 1 DOUBTFUL (i.e. to test in this POC) = this wrapper uses sd::move, what will happen ?!
template <typename Callable, typename Arg>
void wrap_with_move(Arg&& arg) {
    Callable{move(arg)};
}

// INTENTION 2 = in the following cases, the intention is to move (i.e. reuse, invalidate) what was passed :

// INTENTION 2 CORRECT = we use std::move to... well, move :
void pass_a_rref_with_move(vector<int>&& items) {
    Inspect{move(items)};
}

// INTENTION 2 DOUBTFUL (i.e. to test in this POC) = we use std::forward to move, instanciated on T=vector<int>
void pass_a_rref_with_forward_T(vector<int>&& items) {
    Inspect{forward<vector<int>>(items)};
}

// INTENTION 2 DOUBTFUL (i.e. to test in this POC) = we use std::forward to move, instanciated on T&=vector<int>&
void pass_a_rref_with_forward_Tref(vector<int>&& items) {
    Inspect{forward<vector<int>&>(items)};
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
This POC aims to find out when std::move and std::forward differ.

Investigation 1 = what happens if I use a std::move instead of a std::forward ?
(answer = perfect-forwarding is broken, argument is always passed as T&&)

Investigation 2 = what happens if I use a std::forward instead of a std::move ?
(answer = as forward must be explicitly instanciated, it makes no sense (and depends on the explicit instanciation))

What happens, TECHNICALLY :
- std::move always cast into T&&
- std::forward adds && to the instanciated type, then reference collapsing rule apply
- in a non-template context, it depends on what forward is instanciated with
- (if instanciated with the base type T, f std::forward always gives T&&, as std::move does)
- (if instanciated with the type ref T&, f std::forward always gives T&, different from std::move)
- std::forward depends on the instanciated type -> it only makes sense in a templated context

What happens, CONCEPTUALLY :
- std::move    : used to move an object (i.e. pass it as rvalue ref, allowing to steal from it and invalidate it)
- std::forward : used to forward an argument in a templated context (e.g. a template wrapper around a function)
- the INTENT is very different, and properly using move/forward helps convey it

REFERENCE ARTICLE : http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html

ANOTHER INTERESTING ARTICLE : https://www.foonathan.net/2020/09/move-forward/

)DELIM";

    vector<int> items{1, 3, 5, 7, 42};

    cout << "======== INVESTIGATING THE INTENTION 1 = we want to forward the argument :" << endl;
    cout << "Reference case = if we (correcly) use std::forward :" << endl;
    wrap_with_forward<Inspect>(items);  // call with an lvalue -> properly forwarded as an lvalue
    wrap_with_forward<Inspect>(vector<int>{1, 3, 5});
    cout << endl;
    cout << "Invesigated case = if we (doubtfully) use std::move instead of std::forward :" << endl;
    wrap_with_move<Inspect>(items);  // call with an lvalue -> WRONGFULLY forwarded as an rvalue
    wrap_with_move<Inspect>(vector<int>{1, 3, 5});

    cout << endl;
    cout << endl;

    cout << "======== INVESTIGATING THE INTENTION 2 = we want to move an argument :" << endl;
    cout << "Reference case = if we (correcly) use std::move :" << endl;
    pass_a_rref_with_move(vector<int>{2, 2, 2, 4});
    cout << "Invesigated case = if we (doubtfully) use std::forward<vector<int>> instead of std::move :" << endl;
    pass_a_rref_with_forward_T(vector<int>{1, 1, 1});
    cout << "Invesigated case = if we (doubtfully) use std::forward<vector<int>&> instead of std::move :" << endl;
    pass_a_rref_with_forward_Tref(vector<int>{9, 9, 9});

    return 0;
}
