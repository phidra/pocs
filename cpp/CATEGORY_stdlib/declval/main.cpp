#include <iostream>

using namespace std;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = l'utilisation de declval pour récupérer (dans un contexte template) le type de retour d'une fonction.

Le besoin :
    - `call_f_twice_and_return_sum` est une fonction template qui prend un t de type T
    - dans son implémentation, elle appelle t.f()
    - elle renvoie le même type que celui renvoyé par t.f() (et ce type dépend donc de T)

Pour dire "call_f_twice_and_return_sum va renvoyer comme type ce que renvoie f", on a besoin d'exprimer "le type de retour de t.f()".

Problème = pour accéder à "t.f()", il faut une référence sur un objet de son type... C'est ce que permet de récupérer declval.

NOTE IMPORTANTE : il n'y a AUCUNE construction d'objet avec declval<T>() !
L'expression ressemble à une construction mais elle n'est pas évaluée.
(D'ailleurs, on ne peut pas passer de paramètre à la fonction declval : ce n'est pas un constructeur)
La preuve avec le 3ième exemple de la POC :
    - on a un objet dont le constructeur par défaut est deleted (on ne peut pas faire T{})
    - et pourtant on peut appeler declval<T>()

Quelques notes sur ce qu'en dit cppreference : https://en.cppreference.com/w/cpp/utility/declval

> Converts any type T to a reference type, making it possible to use member functions in decltype expressions without the need to go through constructors. 

Même si on semble utiliser une instance d'un objet, on ne "passe" jamais par un constructeur.

> declval is commonly used in templates where acceptable template parameters may have no constructor in common, but have the same member function whose return type is needed.

Dans la POC, Person/Building (qui n'ont qu'un default-constructor) et NoDefaultConstructor (qui n'a qu'un constructeur custom) n'ont pas de constructeur en commun.
Pourtant, on peut utiliser tous ces types de façon identique, et récupérer le type de retour de T.f() (qui est pourtant différent d'un type à l'autre) dans `call_f_twice_and_return_sum` grâce à declval.

> Note that declval can only be used in unevaluated contexts and is not required to be defined; it is an error to evaluate an expression that contains this function.
> [...]  Return value : Cannot be called and thus never returns a value.

Contrairement à ce que la syntaxe laisse à penser, declval<T>().f() n'est PAS évaluée (i.e. on n'appelle PAS la fonction f() sur une instance de T).
L'expression complète utilise decltype :   decltype(declval<T>().f())
Elle se contente de "désigner" le type de retour d'un tel appel, mais sans l'effectuer.

)DELIM";
    cout << endl;
}

template <typename T>
auto call_f_twice_and_return_sum(T const& t) -> decltype(declval<T>().f()) {
    return t.f() + t.f();
}

struct Person {
    int f() const { return 42; }  // for a person, f returns his/her height
};

struct Building {
    string f() const { return "Buffalo"; }  // for a building, f returns its 'city'
};

struct NoDefaultConstructor {
    NoDefaultConstructor(int a_, string b_) : a{a_}, b{b_} {}
    NoDefaultConstructor() = delete;
    int a;
    string b;
    string f() const { return to_string(a) + b; }  // for this class, returns a string that concatenates members
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    Person p{};
    Building b{};

    cout << call_f_twice_and_return_sum(p) << endl;
    ;
    cout << call_f_twice_and_return_sum(b) << endl;
    ;

    /* NoDefaultConstructor x{};  // ne compile pas ! */
    NoDefaultConstructor y(10, "pouet");
    cout << call_f_twice_and_return_sum(y) << endl;
}
