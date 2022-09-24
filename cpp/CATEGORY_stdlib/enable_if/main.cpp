#include <iostream>

#include "entities.h"

using namespace std;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = comment brancher statiquement deux implémentations différentes d'une même fonction en fonction
                          de caractéristique statique sur les types.

Typiquement, je veux faire quelque chose du genre de :

    template <class T>
    void display(T const& t) {
        if ("T a une fonction `toString`") {
            cout << t.toString() << endl;
        }
        else
        {
            cout << "je ne sais pas afficher T !" << endl;
        }
    }

Cette POC détaille les choses pas-à-pas. Elle est très inspirée de :
    https://vittorioromeo.info/index/blog/checking_expression_validity_in_place.html

TL;DR : l'idée générale est :
    - de faire apparaître l'expression `t.toString()` dans une déclaration de type-template
    - ce type template n'existera donc pas si `t.toString()` est ill-formed.
    - d'utiliser plein d'outils (void_t, decltype/declval, true_type/false_type, enable_if, etc.) pour en profiter

)DELIM";
    cout << endl;
}

// =====================================================================================================================
// PREMIÈRE PARTIE = void_t
// TL;DR = my_void_t est un type-template qui vaut toujours void, mais peut inclure une expression template arbitraire.
// Note : le caractère templaté de my_void_t est inutile : peu importe l'argument template, le type vaut toujours void !
// Mais il permet de faire dépendre la définition de my_void_t d'une expression template arbitraire.
// (à noter qu'en C++17, pas besoin de le faire nous-même, ça a été intégré au langage = void_t)

template <typename...>
using my_void_t = void;  // en C++17 on peut utiliser directement void_t

// =====================================================================================================================
// DEUXIÈME PARTIE = decltype + declval :
// decltype permet de récupérer le type d'une expression
// ça nous permet donc de passer d'un objet au type
// ainsi, avec une expression du genre de :
//     decltype(declval<T>().toString())
// On récupère :
//  1. une expression qui désigne un TYPE (et qu'on peut donc passer comme argument template à my_void_t)...
//  2. ... dont la définition, pour être valide, implique la validité de T.toString()

// =====================================================================================================================
// TROISIÈME PARTIE = détecter statiquement si une classe a un membre toString() :
//
// TL;DR : HasToString<T>::type vaut true si T.toString() est valide, et false sinon.
//
// HasToString est une struct-template
//  - à noter qu'ici aussi, les types-templates sont inutiles, et ne sont là que pour permettre de former des
//    expressions qui seront considérées comme invalides par le compilo
//  - side note : on a besoin du premier paramètre template pour l'utiliser dans l'expression passée comme deuxième
//    argument template
// Dans le cas général, HasToString est un false_type (donc elle a le membre ::type à false)
// HasToString a une template-spécialisation du deuxième paramètre template, dérivée de true_type (::type à true).
// L'astuce = cette template spécialisation utilise void_t en incluant une expression template arbitraire, qui nécessite
// que T.toString() soit valide !
//
// Donc en résumé :
//  si T n'a pas de méthode toString, il n'existe qu'une version de HasToString<T>, dont ::type vaut false
//  si T a une méthode toString, il existe DEUX versions de HasToString<T>, l'une avec ::type à false, l'autre à true.
//  Tout ça parce qu'on a réussi à intégrer à la définition du type l'expression dont on voulait tester la validité \o/
//
// Dernière pièce du puzzle : dans le cas où on a le choix entre deux versions possibles (comme c'est le cas ici pour
// HasToString), la règle est de privilégier la version la plus spécialisée :
//      https://en.cppreference.com/w/cpp/language/function_template#Function_template_overloading
//      Informally "A is more specialized than B" means "A accepts fewer types than B".
//
// Ici c'est bien le cas : le HasToString avec ::type à true est retenu car il accepte moins de types que le premier.

template <typename, typename = void>
struct HasToString : false_type {};

template <typename T>
struct HasToString<T, my_void_t<decltype(declval<T>().toString())>> : true_type {};

// =====================================================================================================================
// QUATRIÈME PARTIE = enable_if
//
// https://en.cppreference.com/w/cpp/types/enable_if
//
// enable_if<condition, T=void> est une struct-template à deux arguments (le deuxième à void par défaut) :
//      - si le premier argument est true, enable_if<true, T>::type est un typedef sur T
//      - si le premier argument est false, enable_if<false, T>::type est ill-formed et ne compile pas !
//
// Si on arrive à faire apparaître enable_if<HasToString<T>::type> dans une expression, ça permettra que cette
// expression ne compile QUE si T.toString() est valide.

// =====================================================================================================================
// CINQUIÈME PARTIE = brancher une implémentation différente selon que le type passé en entrée ait toString ou non :
//
// ici, je veux brancher et soit faire appel à toString, soit renvoyer une string par défaut :
//      if ("x.toString() is well-formed") {
//           cout << x.toString() << endl;
//      }
//      else {
//           cout << "UNDISPLAYABLE" << endl;
//      }
//
// Ce qu'on va faire, c'est proposer deux overloads d'une fonction template 'display<T>' :
//     l'un des overloads déclaré ne compilera pas si T n'a pas toString
//     l'autre ne compilera pas si T a toString
// Du coup, selon que T.toString() est valide ou non, seul l'un des deux overloads à la fois sera visible.
//
// L'astuce : pour cela, on va faire apparaître quelque part dans l'expression de la fonction-template le fameux
// enable_if<HasToString<T>>::type (qui vaut ce qu'on veut, ici void)...
// Et ce "quelque-part", ça va être dans le type de retour, via la syntaxe  auto f(...) -> return-type :
//
// TL;DR : on une fonction template display<T> qui a deux overloads qui ne sont jamais well-formed en même temps.

template <typename T>
auto display(T const& x) -> typename enable_if<HasToString<T>{}>::type {
    cout << x.toString() << endl;
}

template <typename T>
auto display(T const& x) -> typename enable_if<!HasToString<T>{}>::type {
    cout << "UNDISPLAYABLE : " << &x << endl;
}

// caveat : ici, je ne peux PAS ajouter une fonction catchall dans ce genre-là :
//     template <typename T>
//     void display(T const& x) {
//         cout << "UNDISPLAYABLE : " << &x << endl;
//     }
// En effet, pour les types qui ont toString, le compilo aurait alors DEUX implémentations possibles
// avec exactement la même signature, et on aurait cette erreur :
//    main.cpp:86:12: error: call of overloaded ‘display(Person&)’ is ambiguous
//       86 |     display(p1);
//          |     ~~~~~~~^~~~
//
// NDM : j'ai du mal à comprendre pourquoi l'une des deux implémentations n'est pas considérée comme
// plus spécialisée que l'autre, vu que si le type n'a pas hasToString, enable_if est mal formé...

// =====================================================================================================================
// ALTERNATIVE PLUS SIMPLE MAIS MOINS PUISSANTE :
//
// Ce qui suit n'est PAS le point principal de la POC  (centrée sur le cas générique avec enable_if).
// Mais on pourrait se dire "pas besoin de enable_if, on peut faire la même chose en template-specialisant display" ?
//
// Et en effet, ça fonctionne :

template <typename T>
void alternative(T const& x) {
    // dans le cas template général, on utilise toString :
    cout << x.toString() << endl;
}

template <>
void alternative<Graph>(Graph const& x) {
    // on spécialise pour Graph (qui n'a pas toString)
    cout << "UNDISPLAYABLE : " << &x << endl;
}

// Et certes, c'est plus simple... Mais c'est moins puissant, car moins générique !
// En effet, cette alternative nécessite de template-spécialiser pour chaque type que je veux display.
// (notamment, pour des types que je ne connais pas encore, je suis bloqué)
// A contrario, la solution avec enable_if marchera avec toute classe, selon qu'elle ait ou non toString.

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    Person p1("Luke", "Skywalker", 42);
    Person p2("Leia", "Skywalker", 42);
    Building death_star("Far far away, Universe", 9999);
    Graph g(100, 120);  // <-- n'a pas de méthode "toString" !

    cout << endl;
    cout << "=== avec enable_if :" << endl;
    display(p1);
    display(p2);
    display(death_star);
    display(g);

    // ALTERNATIVE PLUS SIMPLE MAIS MOINS PUISSANTE :
    cout << endl;
    cout << "=== alternative plus simple mais moins puissante :" << endl;
    alternative(p1);
    alternative(p2);
    alternative(death_star);
    alternative(g);
    // à noter que dans le dernier cas, c'est bien la template-spécialisation qui est retenue, pour les mêmes raisons
    // que documenté plus haut = car c'est "la version la plus spécialisée"

    // on peut montrer que cette alternative est moins puissante en ajoutant un nouveau type qui n'a pas toString :
    cout << endl;
    cout << "=== démonstration du caractère 'moins puissant' sur un nouveau type :" << endl;
    struct NewType {};
    NewType x{};
    display(x);  // OK : sans avoir eu besoin de modifier 'display', ceci compile et fonctionne comme attendu
    /* alternative(x);  // KO : ne compile pas, car la template-specialisation alternative<NewType> n'existe pas ! */
}
