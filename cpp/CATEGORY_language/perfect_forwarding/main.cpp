#include <iostream>

#include "objects_to_forward_to.h"
#include "wrap1.h"
#include "wrap2.h"
#include "wrap3.h"
#include "wrap4.h"
#include "wrap5_perfect_forwarding.h"

using namespace std;

void assert(bool asserted_condition1, bool asserted_condition2 = true) {
    if (!asserted_condition1 || !asserted_condition2) {
        cout << "ERROR : the POC does not behave like expected !" << endl;
        exit(1);
    }
}

void this_is_an_UNDESIRED_behaviour(bool behaviour1, bool behaviour2 = true) {
    // with this function (that also "asserts" the behaviours), we MARK the behaviours as not desired
    // this function is used to show that e.g. wrap1 is not sufficient to implement forwarding

    // even if the behaviours are undesirable, this is still what the POC does -> we enforce it :
    assert(behaviour1 && behaviour2);
}

void display_the_desired_behaviour_with_DIRECT_call_to_Inspector() {
    int i = 10;
    int& i_ref = i;
    int const& i_cref = i;

    int j = 20;
    int& j_ref = j;
    int const& j_cref = j;

    assert(i == 10, j == 20);

    // all the calls with j :
    assert(Inspector(i, j).was_called_with_ref_ref);
    assert(i == 11, j == 21);

    assert(Inspector(i_ref, j).was_called_with_ref_ref);
    assert(i == 12, j == 22);

    assert(Inspector(i_cref, j).was_called_with_cref_ref);
    assert(i == 12, j == 23);

    assert(Inspector(42, j).was_called_with_rref_ref);
    assert(i == 12, j == 24);

    // all the calls with j_ref :
    assert(Inspector(i, j_ref).was_called_with_ref_ref);
    assert(i == 13, j == 25);

    assert(Inspector(i_ref, j_ref).was_called_with_ref_ref);
    assert(i == 14, j == 26);

    assert(Inspector(i_cref, j_ref).was_called_with_cref_ref);
    assert(i == 14, j == 27);

    assert(Inspector(42, j_ref).was_called_with_rref_ref);
    assert(i == 14, j == 28);

    // all the calls with j_cref :
    assert(Inspector(i, j_cref).was_called_with_ref_cref);
    assert(i == 15, j == 28);

    assert(Inspector(i_ref, j_cref).was_called_with_ref_cref);
    assert(i == 16, j == 28);

    assert(Inspector(i_cref, j_cref).was_called_with_cref_cref);
    assert(i == 16, j == 28);

    assert(Inspector(42, j_cref).was_called_with_rref_cref);
    assert(i == 16, j == 28);

    // all the calls with j_rref :
    assert(Inspector(i, 100).was_called_with_ref_rref);
    assert(i == 17, j == 28);

    assert(Inspector(i_ref, 100).was_called_with_ref_rref);
    assert(i == 18, j == 28);

    assert(Inspector(i_cref, 100).was_called_with_cref_rref);
    assert(i == 18, j == 28);

    assert(Inspector(42, 100).was_called_with_rref_rref);
    assert(i == 18, j == 28);
}

void see_if_wrap1_does_the_work___spoiler_it_does_not() {
    cout << endl;
    cout << "=== WRAP 1, which does NOT behave as required, because it cannot mutate its arguments" << endl;
    // This first wrapper captures its elements by-value.
    // Whatever the way we call wrap1, the passed i and j are NEVER mutated (because passed by value in wrap1)
    // Moreover, if we pass a cref or rref, the proper version of Inspector is NOT called
    int i = 10;
    int j = 20;
    int& i_ref = i;
    int const& i_cref = i;
    assert(wrap1<Inspector>(i, j).was_called_with_ref_ref);
    this_is_an_UNDESIRED_behaviour(i == 10, j == 20);

    assert(wrap1<Inspector>(i_ref, j).was_called_with_ref_ref);
    this_is_an_UNDESIRED_behaviour(i == 10, j == 20);

    this_is_an_UNDESIRED_behaviour(wrap1<Inspector>(i_cref, j).was_called_with_ref_ref);
    this_is_an_UNDESIRED_behaviour(i == 10, j == 20);

    this_is_an_UNDESIRED_behaviour(wrap1<Inspector>(42, j).was_called_with_ref_ref);
    this_is_an_UNDESIRED_behaviour(i == 10, j == 20);
}

void see_if_wrap2_does_the_work___spoiler_it_does_not() {
    cout << endl;
    cout << "=== WRAP 2, which does NOT behave as required, because we cannot call it with temporaries" << endl;
    // When compared to wrap1, this second wrapper captures its element by non-const reference.
    // It is better than wrap1 (references are mutated when non-const, and left unchagned when const).
    // But it is still insufficient : calling this wrapper with temporaries does not compile.
    // (which is logical : the wrapper expects its arguments as non-const references, which cannot bind on temporaries)
    int i = 10;
    int j = 20;
    int& i_ref = i;
    int const& i_cref = i;

    assert(wrap2<Inspector>(i, j).was_called_with_ref_ref);
    assert(i == 11, j == 21);

    assert(wrap2<Inspector>(i_ref, j).was_called_with_ref_ref);
    assert(i == 12, j == 22);

    assert(wrap2<Inspector>(i_cref, j).was_called_with_cref_ref);
    assert(i == 12, j == 23);

    // if uncommented, the following line does NOT compile :
    /* wrap2<Inspector>(42, j); */

    // but the DIRECT call DOES compile :
    assert(Inspector(42, j).was_called_with_rref_ref);
    assert(i == 12, j == 24);

    // thus, wrap2 fails to properly wrap the call, QED
}

void see_if_wrap3_does_the_work___spoiler_it_does_not() {
    cout << endl;
    cout << "=== WRAP 3, which does NOT behave as required, because it cannot mutate its arguments" << endl;
    // When compared to wrap2, this third wrapper captures its element by const reference.
    // This allows to pass temporaires BUT turns every input non-const ref into a const-ref.
    // Thus, all the calls are wrapped into a cosntruction with const-ref : arguments cannot be mutated.

    int i = 10;
    int j = 20;
    int& i_ref = i;
    int const& i_cref = i;

    this_is_an_UNDESIRED_behaviour(wrap3<Inspector>(i, j).was_called_with_cref_cref);
    this_is_an_UNDESIRED_behaviour(i == 10, j == 20);

    this_is_an_UNDESIRED_behaviour(wrap3<Inspector>(i_ref, j).was_called_with_cref_cref);
    this_is_an_UNDESIRED_behaviour(i == 10, j == 20);

    this_is_an_UNDESIRED_behaviour(wrap3<Inspector>(i_cref, j).was_called_with_cref_cref);
    this_is_an_UNDESIRED_behaviour(i == 10, j == 20);

    this_is_an_UNDESIRED_behaviour(wrap3<Inspector>(42, j).was_called_with_cref_cref);
    this_is_an_UNDESIRED_behaviour(i == 10, j == 20);
}

void see_if_wrap4_does_the_work___spoiler_it_does_but_is_very_tedious() {
    cout << endl;
    cout << "=== WRAP 4, which does behave as required, but is VERY tedious" << endl;
    int i = 10;
    int& i_ref = i;
    int const& i_cref = i;

    int j = 20;
    int& j_ref = j;
    int const& j_cref = j;

    assert(i == 10, j == 20);

    // all the calls with j :
    assert(wrap4<Inspector>(i, j).was_called_with_ref_ref);
    assert(i == 11, j == 21);

    assert(wrap4<Inspector>(i_ref, j).was_called_with_ref_ref);
    assert(i == 12, j == 22);

    assert(wrap4<Inspector>(i_cref, j).was_called_with_cref_ref);
    assert(i == 12, j == 23);

    assert(wrap4<Inspector>(42, j).was_called_with_rref_ref);
    assert(i == 12, j == 24);

    // all the calls with j_ref :
    assert(wrap4<Inspector>(i, j_ref).was_called_with_ref_ref);
    assert(i == 13, j == 25);

    assert(wrap4<Inspector>(i_ref, j_ref).was_called_with_ref_ref);
    assert(i == 14, j == 26);

    assert(wrap4<Inspector>(i_cref, j_ref).was_called_with_cref_ref);
    assert(i == 14, j == 27);

    assert(wrap4<Inspector>(42, j_ref).was_called_with_rref_ref);
    assert(i == 14, j == 28);

    // all the calls with j_cref :
    assert(wrap4<Inspector>(i, j_cref).was_called_with_ref_cref);
    assert(i == 15, j == 28);

    assert(wrap4<Inspector>(i_ref, j_cref).was_called_with_ref_cref);
    assert(i == 16, j == 28);

    assert(wrap4<Inspector>(i_cref, j_cref).was_called_with_cref_cref);
    assert(i == 16, j == 28);

    assert(wrap4<Inspector>(42, j_cref).was_called_with_rref_cref);
    assert(i == 16, j == 28);

    // all the calls with j_rref :
    assert(wrap4<Inspector>(i, 100).was_called_with_ref_rref);
    assert(i == 17, j == 28);

    assert(wrap4<Inspector>(i_ref, 100).was_called_with_ref_rref);
    assert(i == 18, j == 28);

    assert(wrap4<Inspector>(i_cref, 100).was_called_with_cref_rref);
    assert(i == 18, j == 28);

    assert(wrap4<Inspector>(42, 100).was_called_with_rref_rref);
    assert(i == 18, j == 28);
}

void see_if_wrap5_does_the_work___spoiler_it_does_and_this_is_perfect_forwarding() {
    cout << endl;
    cout << "=== WRAP 5, which does behave as required, and is the canonical perfect forwarding for an arity of 2"
         << endl;
    int i = 10;
    int& i_ref = i;
    int const& i_cref = i;

    int j = 20;
    int& j_ref = j;
    int const& j_cref = j;

    assert(i == 10, j == 20);

    // all the calls with j :
    assert(wrap5<Inspector>(i, j).was_called_with_ref_ref);
    assert(i == 11, j == 21);

    assert(wrap5<Inspector>(i_ref, j).was_called_with_ref_ref);
    assert(i == 12, j == 22);

    assert(wrap5<Inspector>(i_cref, j).was_called_with_cref_ref);
    assert(i == 12, j == 23);

    assert(wrap5<Inspector>(42, j).was_called_with_rref_ref);
    assert(i == 12, j == 24);

    // all the calls with j_ref :
    assert(wrap5<Inspector>(i, j_ref).was_called_with_ref_ref);
    assert(i == 13, j == 25);

    assert(wrap5<Inspector>(i_ref, j_ref).was_called_with_ref_ref);
    assert(i == 14, j == 26);

    assert(wrap5<Inspector>(i_cref, j_ref).was_called_with_cref_ref);
    assert(i == 14, j == 27);

    assert(wrap5<Inspector>(42, j_ref).was_called_with_rref_ref);
    assert(i == 14, j == 28);

    // all the calls with j_cref :
    assert(wrap5<Inspector>(i, j_cref).was_called_with_ref_cref);
    assert(i == 15, j == 28);

    assert(wrap5<Inspector>(i_ref, j_cref).was_called_with_ref_cref);
    assert(i == 16, j == 28);

    assert(wrap5<Inspector>(i_cref, j_cref).was_called_with_cref_cref);
    assert(i == 16, j == 28);

    assert(wrap5<Inspector>(42, j_cref).was_called_with_rref_cref);
    assert(i == 16, j == 28);

    // all the calls with j_rref :
    assert(wrap5<Inspector>(i, 100).was_called_with_ref_rref);
    assert(i == 17, j == 28);

    assert(wrap5<Inspector>(i_ref, 100).was_called_with_ref_rref);
    assert(i == 18, j == 28);

    assert(wrap5<Inspector>(i_cref, 100).was_called_with_cref_rref);
    assert(i == 18, j == 28);

    assert(wrap5<Inspector>(42, 100).was_called_with_rref_rref);
    assert(i == 18, j == 28);
}

void see_if_our_wrapper_can_work_with_another_callable() {
    cout << endl;
    cout << "=== WRAP 5, but used with another callable" << endl;
    int i = 10;
    int& i_ref = i;
    int const& i_cref = i;

    string s{"coucou"};
    string& s_ref = s;
    string const& s_cref = s;

    assert(s == "coucou", i == 10);

    assert(wrap5<AnotherInspector>(s_cref, i_ref).was_called_with_cref_ref);
    assert(s == "coucou", i == 11);

    assert(wrap5<AnotherInspector>(s_ref, i_cref).was_called_with_ref_cref);
    assert(s == "coucou_pouet", i == 11);

    assert(wrap5<AnotherInspector>(s_ref, i_ref).was_called_with_ref_ref);
    assert(s == "coucou_pouet_pouet", i == 12);

    assert(wrap5<AnotherInspector>(string("hohoho"), i_cref).was_called_with_rref_cref);
    assert(s == "coucou_pouet_pouet", i == 12);

    // actually, with AnotherInspector, constructing with a temporary fallbacks on cref :
    assert(wrap5<AnotherInspector>(string("hohoho"), 42).was_called_with_rref_cref);
    assert(s == "coucou_pouet_pouet", i == 12);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
This POC has objectives that revolves around how to build a wrapper that forwards its arguments to a wrapped-function :
    1. it settles the problem that we are trying to solve
    2. give a real instance of this problem
    3. it show the solution = how to implement PERFECT-FORWARDING
    4. it show several alternative implemnentations (that don't work or are less efficient)

This POC is heavily inspired by :
    https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c

TL;DR : see the proper implementation in 'wrap5_perfect_forwarding.h'

===== OBJECTIVE 1 = problem settling :

The problem we are trying to solve = we want a WRAPPER that can wrap the construction of any object.
There is a constraint : as we want to avoid variadic-templates, all the constructions will expect exactly 2 arguments.
Both arguments must be arbitrary : our wrapper must accept any object...
... at the condition that its constructor expects 2 arguments, whatever they are.
(in the POC, we provide 2 such objects : Inspector and AnotherInspector)

e.g. Inspector has these 2 constructors, that behave differently :
    Inspector(int&, int&);
    Inspector(int const&, int&&);

We want our wrapper to guarantee that if we pass 'int& int&', the first constructor will be called...
... and we pass 'int const&, int&&', the second constructor will be called.

More generally, the problem is to be able to wrap an overload-set of callables, where the value-types of the argument
are used to resolve the overload-set. For instance, a DIFFERENT constructor will be called for 'int&&' and 'int const&'

Of course, as the the wrapper must work for any object, with any parameters, its implementation must be GENERIC.

===== OBJECTIVE 2 = give a real instance of this problem :

Du coup, fonctionnellement (à démontrer sans wrapper) :
    j'ai 3 possibilités pour chaque type, deux types, donc 9 possibilités de construction
    je PEUX passer une référence constante au constructeur
        si je passe une référence constante, la construction se fait sans muter l'input
        si je passe une référence NON-CONSTANTE, la construction MUTE l'input
    je PEUX passer une rvalue-référence au constructeur

Mentionner que pour bien montrer que ça marche avec autre chose, je donne AnotherInspector

===== OBJECTIVE 3 = show the solution :

The proper implementation for our problem is given in "wrap5_perfect_forwarding.h".
It relies on the combination of 3 things :
    - universal references (i.e. "T&& in a type-deduction context", which has a special deduction rule)
    - std::forward as a mean to cast a T to T&&
    - reference-collapsing, that ensures that T& && = T&  and T&& && = T&&

===== OBJECTIVE 4 = show alternative implementations

The wrappers 1 to 4 show trials to achive perfect-forwarding.
The first 3 wrappers fail to implement proper forwarding.
The fourth wrapper actually succeeds, but require an (exponential) number of template-specialization, so is unusable.

)DELIM";

    // PRELIMINARIES : the following displays the desired behaviour, when calling Inspector DIRECTLY
    // (and the goal of the POC is to obtain exactly the same behaviour when calling it INDIRECTLY, through a wrapper)
    display_the_desired_behaviour_with_DIRECT_call_to_Inspector();

    see_if_wrap1_does_the_work___spoiler_it_does_not();
    see_if_wrap2_does_the_work___spoiler_it_does_not();
    see_if_wrap3_does_the_work___spoiler_it_does_not();
    see_if_wrap4_does_the_work___spoiler_it_does_but_is_very_tedious();
    see_if_wrap5_does_the_work___spoiler_it_does_and_this_is_perfect_forwarding();
    see_if_our_wrapper_can_work_with_another_callable();

    cout << endl;

    return 0;
}
