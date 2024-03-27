#include <iostream>

using namespace std;

// Dummy example of SFINAE usage (not the smartest one, wikipedia's is more interesting)

// We have a bunch of classes that has some content.
// Some expose their content through "get_value", other through "get_the_content".
struct A1 {
    int get_value() const { return 42; }
};
struct A2 {
    int get_value() const { return 142; }
};
struct A3 {
    int get_value() const { return 1142; }
};
struct B1 {
    int get_the_content() const { return 10; }
};
struct B2 {
    int get_the_content() const { return 100; }
};
struct B3 {
    int get_the_content() const { return 1000; }
};

// We'd like to display the content of all A* / B*, with an overloaded template-function.
// How, since A* and B* interfaces are different ?
// We provide two template-functions, SFINAE allows compiler to choose the one to use.

template <typename T, class Ignored = decltype(declval<T>().get_value())>
void display(const T& t) {
    cout << "Here is A* content : " << t.get_value() << endl;
}

template <typename T, class Ignored = decltype(declval<T>().get_the_content()), class EvenMoreIgnored = bool>
void display(const T& t) {
    cout << "Here is B* content : " << t.get_the_content() << endl;
}

// Here, SFINAE is :
//  - the first templated-definition will statically fail on a B* type (because B has no "get_value" member, which is
//  used on template-definition)
//  - the second templated-definition will statically fail on a A* type (idem, but with "get_the_content" member)
//  - thus, the compiler use either defintion, if it displays A* or B*, AT COMPILE TIME.
//  - again, this is to illustrate how SFINAE works : there would be better ways to display A* or B*.

int main(int argc, char* argv[]) {
    display(A1{});
    display(A2{});
    display(A3{});
    display(B1{});
    display(B2{});
    display(B3{});
    return 0;
}
