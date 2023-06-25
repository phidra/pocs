#include <iostream>
#include <vector>
#include <typeinfo>

using std::cout;
using std::endl;
using std::vector;


// Cette POC montre l'utilisation (et notamment l'initialisation) de membre statique d'une classe.
// À noter qu'on peut utiliser les membres (et méthodes) statiques d'une classe même sans qu'elle soit jamais instanciée.
// Pour initialiser un membre statique, tout se passe comme si on le déclarait une seconde fois.
// See: https://blog.tartanllama.xyz/initialization-is-bonkers/#fnref:1


struct Storage {

    static vector<int> _data;  // membre statique -> utilisable sans jamais instancier la classe

    static void add(const int& t) { cout << "... adding : " << t << endl; _data.push_back(t); }
    static void display() {
        cout << "Contenu de storage :" << endl;
        for (const int& x: _data)
        {
            cout << "\t - " << x << endl;
        }
    }
};

// À ce stade, on ne peut pas encore utiliser Storage (erreur au link), car son membre _data n'a pas été défini.


vector<int> Storage::_data;  // définit _data (et autorise donc le link)

// grâce à la ligne précédente, _data sera d'abord zero-initialized, puis default-initilized :
// https://en.cppreference.com/w/cpp/language/default_initialization
// https://en.cppreference.com/w/cpp/language/zero_initialization


int main(void)
{
    Storage::add(43);
    Storage::add(44);
    Storage::add(45);
    Storage::display();

    return 0;
}
