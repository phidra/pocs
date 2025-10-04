#include <iostream>

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = le pourquoi des lambdas mutable :

1. Une lambda qui capture par référence peut muter une variable capturée.
   (dit autrement, la capture par référence se fait par référence non-const)
2. Une lambda qui capture par copie n'a pas le droit de muter la variable capturée, même copiée.
   pour avoir le droit de la muter, la lambda doit être déclarée `mutable`

La doc cppreference a plein d'infos utiles :
   https://en.cppreference.com/w/cpp/language/lambda.html

J'en profit pour illustrer qu'une méthode const a le droit de muter une référence non-const.
(car c'est connexe au fait que par défaut, `operator()` de la lambda est const)
)DELIM";
    cout << endl;
}

// Structure qui a une référence mutable, mais une méthode const
// Elle servira à montrer qu'une méthode const peut muter une référence mutable.
struct Pouet {
    Pouet(std::string& name) : ref_to_name{name} {}
    void this_is_a_const_method() const { ref_to_name = "Look, I've been mutated !"; }
    std::string& ref_to_name;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    std::string name = "Luke";

    // captures par copie et par référence :
    auto does_not_mutate__by_value = [name]() { cout << "Hello, " << name << endl; };
    auto does_not_mutate__by_ref = [&name]() { cout << "Hello, " << name << endl; };

    // comme on ne mute pas la variable capturée, on peut utiliser les deux sans `mutable` :
    does_not_mutate__by_value();
    does_not_mutate__by_ref();

    // captures par copie et par référence :
    auto does_mutate__by_ref = [&name]() {
        name = "Leia";
        cout << "Hello, " << name << endl;
    };

    // si on enlève le keyword `mutable`, ça ne compile plus !
    auto does_mutate__by_value = [name]() mutable {
        name = "Yoda";
        cout << "Hello, " << name << endl;
    };

    // en ayant capturé par référence, la lambda modifie la string référencée :
    cout << endl << endl;
    cout << "==== Avant `does_mutate__by_ref`, que vaut name = " << name << endl;
    does_mutate__by_ref();
    cout << "==== Apres `does_mutate__by_ref`, que vaut name = " << name << endl;

    // en ayant capturé par copie, la lambda ne modifie que sa copie interne :
    // (et ce qui est contre-intuitif, c'est que pour modifier cet état interne, il faut `mutable` !)
    cout << endl << endl;
    cout << "==== Avant `does_mutate__by_value`, que vaut name = " << name << endl;
    does_mutate__by_value();
    cout << "==== Apres `does_mutate__by_value`, que vaut name = " << name << endl;

    // sujet connexe = montrer qu'une méthode const peut muter une référence non-constante :
    cout << endl << endl;
    cout << "Avant `this_is_a_const_method`, que vaut name = " << name << endl;
    Pouet p{name};
    p.this_is_a_const_method();
    cout << "Après `this_is_a_const_method`, que vaut name = " << name << endl;

    return 0;
}
