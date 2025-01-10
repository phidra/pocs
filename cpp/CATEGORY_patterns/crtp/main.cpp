#include <iostream>

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = deux exemples de CRTP et leurs usages.

PRINCIPE DU CRTP = une classe fille hérite de la classe parente templatée par elle-même :
    template <typename Derived> struct Parent { /* ... */ };
    struct Child: public Parent<Child> { /* ... */ };


USAGE N°1 = interfaces statiques *explicites*
    - si une fonction (e.g. explain_animal) a besoin de travailler avec des types différents, mais homogènes
      elle peut être une fonction template ; dans ce cas, elle définit une interface statique IMPLICITE (i.e.
      tout type qui satisfait les contraintes templates pourra être passé à la fonction)
    - ça n'est pas forcément ce qu'on veut : on peut vouloir qu'une fonction marche avec une famille de types
      différents mais homogènes, mais uniquement ceux qui ont choisi de faire EXPLICITEMENT partie de la famille.
    - dit autrement, on ne veut pas qu'un type puisse satisfaire la contrainte template "par hasard"
    - le CRTP permet une interface statique explicite :
        - la façon dont le parent (Animal) utilise les enfants (Dog/Fish) définit une sorte d'interface statique
        - les enfants (Dog / Fish) "implémentent" cette interface en implémentant ce dont a besoin le Parent
        - la fonction (explain_animal) manipule de la même façon les différents Animal<Derived>
    - [ce talk](https://www.youtube.com/watch?v=pmdwAf6hCWg) déconseille cet usage, notamment car :
        - les classes filles n'héritent pas d'un même type de base, donc elles ne peuvent pas *vraiment* être
          manipulées de façon homogène (par exemple, il est impossible de faire un std::vector<Animal>)
        - pour manipuler des Animal, toute fonction doit être template -> tout mon code finit par être template


USAGE N°2 = mixin :
    - on veut ajouter des fonctionnalités à une classe, en la faisant hériter de la mixin
    - ici, le fait pour Length d'hériter de NumericalFunctions nous donne accès à ses fonctions (qui sont utiles)
    - NDM : il y a un parallèle avec les traits, sauf que le trait crée plutôt une nouvelle classe utilisant le type
      pour ajouter la fonctionnalité, alors qu'avec le CRTP, on hérite d'une classe utilisant le type pour ajouter
      la fonctionnalité


Le CRTP se repose sur le fait que le parent est lié à l'enfant par un contrat = la façon dont le parent utilise l'enfant.
Dit autrement : que doivent respecter les classes filles pour être utilisée par le parent en tant que Derived?
    - p.ex. pour l'usage n°1 toute classe fille doit obligatoirement proposer une méthode "get_the_home"
    - p.ex. pour l'usage n°2 toute classe fille doit obligatoirement proposer une méthode "getValue"


note : j'ai fait cette POC suite à ce talk :
    https://github.com/phidra/notes/blob/main/2025-01-06-cpp-design-patterns-misconceptions.md

)DELIM";
    cout << endl;
}

// USAGE N°1 DU CRTP = interface statique :
template <typename Derived>
struct Animal {
    // ici, "l'interface" est le contrat que doit remplir Derived pour être utilisé (get_the_home / make_the_noise)
    std::string get_home() const { return static_cast<Derived const*>(this)->get_the_home(); }
    void make_noise() const { return static_cast<Derived const*>(this)->make_the_noise(); }

   private:
    // sachant qu'on static_cast en Derived, c'est important que n'importe qui ne puisse pas créer un Animal
    // (la construction d'un Animal doit être réservée à un vrai Derived, i.e. héritant de Animal<Derived>)
    Animal<Derived>() {}
    friend Derived;  // nécessaire pour que Derived puisse construire un Animal<Derived>
};

// Dog et Fish "implémentent" l'interface (i.e. respectent le contrat template que Animal nécessite)

struct Dog : public Animal<Dog> {
    std::string get_the_home() const { return "à la maison"; }
    void make_the_noise() const { std::cout << "Ouaf" << std::endl; }
};

struct Fish : public Animal<Fish> {
    std::string get_the_home() const { return "dans la mer"; }
    void make_the_noise() const { std::cout << "Blub" << std::endl; }
};

// enfin, une fonction template pour travailler avec un animal (et uniquement un animal : pas n'importe quelle classe
// qui respecte le template) :
template <typename Derived>
void explain_animal(Animal<Derived> const& animal) {
    std::cout << "Vois-tu, cet animal vit : " << animal.get_home() << std::endl;
    std::cout << "Et quand il parle, ça fait :" << std::endl;
    animal.make_noise();
    std::cout << std::endl;
}

// USAGE N°2 DU CRTP = mixin (exemple piqué de https://www.fluentcpp.com/2017/05/16/what-the-crtp-brings-to-code/) :
template <typename Derived>
struct NumericalFunctions {
    // ici, les fonctionnalités ajoutées par la Mixin utilisent les fonctions setValue/getValue des filles
    void scale(double multiplicator) { self()->setValue(self()->getValue() * multiplicator); }
    void square() { self()->setValue(self()->getValue() * self()->getValue()); }
    void setToOpposite() { self()->scale(-1); };
    Derived* self() { return static_cast<Derived*>(this); }
};

struct Sensitivity : NumericalFunctions<Sensitivity> {
    double getValue() const { return sensitivity; }
    void setValue(double value) { sensitivity = value; }
    double sensitivity;
    Sensitivity(double sensitivity_) : sensitivity{sensitivity_} {}
};

struct Length : NumericalFunctions<Length> {
    double getValue() const { return length; }
    void setValue(double value) { length = value; }
    double length;
    Length(double length_) : length{length_} {}
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    // usage n°1 = avoir une interface statique plus explicite que les templates (qui sont une interface implicite)
    Fish fish;
    Dog dog;
    explain_animal(fish);
    explain_animal(dog);
    std::cout << "USAGE n°1 : OK" << std::endl;

    // usage n°2 = ajouter des fonctionnalités à une classe (mixin) :
    Sensitivity sensitivity{42};
    sensitivity.scale(2);
    if (sensitivity.getValue() != 84) {
        throw std::runtime_error("unexpected scaled sensitivity value");
    }
    Length length{100};
    length.scale(3);
    if (length.getValue() != 300) {
        throw std::runtime_error("unexpected scaled length value");
    }
    std::cout << "USAGE n°2 : OK" << std::endl;
}
