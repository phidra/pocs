#include <iostream>
#include <typeinfo>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

// Par rapport à la POC précédente, cette POC ajoute le côté "template" à la classe qui a des membre/méthode statiques.
// Notamment, elle montre la syntaxe pour définir (initializer) le membre template statique.
// Le point que je trouvais curieux, c'est qu'on n'a besoin de définir qu'une seule fois les membres templates
// statiques. (mais après tout c'est logique : on n'a également besoin de ne définir qu'une seule fois les fonctions ou
// méthodes templates) See: https://blog.tartanllama.xyz/initialization-is-bonkers/#fnref:1

template <class T>
struct Storage {
    static vector<T> _data;
    static void add(const T& t) {
        cout << "... adding : " << t << endl;
        _data.push_back(t);
    }
    static void display() {
        cout << "contendu de storage<" << typeid(T).name() << ">   : " << endl;
        for (auto const& x : _data) {
            cout << "\t - " << x << endl;
        }
    }
};

template <class T>
vector<T> Storage<T>::_data;  // une seule initialization pour toutes les versions templates du membre !

int main(void) {
    Storage<int>::add(43);
    Storage<int>::add(44);
    Storage<int>::add(45);
    Storage<int>::display();

    cout << endl << endl;

    Storage<string>::add("youpi 1");
    Storage<string>::add("youpi 2");
    Storage<string>::add("youpi 3");
    Storage<string>::display();

    return 0;
}
