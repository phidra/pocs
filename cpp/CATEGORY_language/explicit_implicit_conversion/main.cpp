#include <iostream>
#include <typeinfo>
#include <string>

using std::cout;
using std::endl;
using std::string;


// Keywords :
//      - converting constructor
//      - copy initialization
//      - implicit conversion
//      - explicit specifier

// Si un constructeur à un seul paramètre n'est pas déclaré explicit, des conversions implicites peuvent avoir lieu.
// La fonction f n'accepte que des Implicit, pourtant on peut lui passer des ints : le compilo convertit implicitement l'int en Implicit
// Par ailleurs, on peut utiliser la copy initialization (alors qu'avec un Explicit, ça ne marche pas).

class Implicit
{
    public:
        Implicit(int arg) : member(arg) { cout << "+++ IMPLICIT constructor called with : " << arg << endl; }
        void display() const { cout << "Implicit::member is : " << member << endl; }
    private:
        int member = 42;
};

class Explicit
{
    public:
        explicit Explicit(int arg) : member(arg) { cout << "+++ EXPLICIT constructor called with : " << arg << endl; }
        void display() const { cout << "Explicit::member is : " << member << endl; }
    private:
        int member = 42;
};


void f(const Implicit& arg)
{
    cout << "===== appel de f (qui attend un Implicit) avec un paramètre de type : " << typeid(arg).name() << " =====" << endl;
    arg.display();
    cout << endl;
}


void g(const Explicit& arg)
{
    cout << "===== appel de g (qui attend un Implicit) avec un paramètre de type : " << typeid(arg).name() << " =====" << endl;
    arg.display();
    cout << endl;
}


int main(int argc, char* argv[])
{
    // f attend un Implicit, on peut donc lui passer un int : comme il existe un constructeur de conversion de int vers Implicit, ça compile :
    f(50);
    // on peut aussi passer d'autres types :
    f(51.2);  // à noter que f convertit le float en int -> on perd en précision
    f('a');
    Implicit c1 = 1000;  // par ailleurs, on peut construire un Implicit en utilisant la copy-initialization :
    Implicit c2(2000);   // la direct initialization est (bien-sûr) utilisable aussi.

    // g attend un Explicit, on ne peut rien lui passer d'autre qu'un Explicit :
    // g(50);     // ne compile pas !
    // g(51.2);   // ne compile pas !
    // g('a');    // ne compile pas !
    // // par ailleurs, on ne peut pas construire un Explicit en utilisant la copy-initialization :
    // Explicit c3 = 1000;
    Explicit c4(2000);  // en revanche, on peut toujours utiliser la direct initialization
}
