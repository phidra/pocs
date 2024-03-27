#include <iostream>

using namespace std;

// cette classe est juste là pour logger ses constructions / destructions
struct A {
    A() { cout << "-- constructor" << endl; }
    ~A() { cout << "-- destructor" << endl; }
};

A f() {
    A var = A{};
    return var;
}  // cette fonction se contente de renvoyer une variable temporaire

int main(void) {
    {
        // La ligne ci-dessous ne compile pas :
        // clang++  :  non-const lvalue reference to type 'A' cannot bind to a temporary of type 'A'
        // g++      :  invalid initialization of non-const reference of type ‘A&’ from an rvalue of type ‘A’
        // A& var0 = f();

        // En revanche, la ligne ci-dessous compile, et ÉTEND LA PORTÉE DE LA VARIABLE TEMPORAIRE jusqu'à la fin du
        // scope !
        const A& var1 = f();
        cout << "Fin du scope 1" << endl;
    }

    cout << endl;

    {
        // Dans le cas "classique", on voit que la variable temporaire est détruite bien avant la fin du scope :
        f();
        cout << "Fin du scope 2" << endl;
    }
}
