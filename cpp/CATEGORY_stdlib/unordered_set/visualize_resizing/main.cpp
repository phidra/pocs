#include <iostream>
#include <numeric>
#include <unordered_set>

using namespace std;

void display(unordered_set<int> const& myset) {
    auto f = [&](string s, int x) {
        string r = move(s);
        if (!r.empty()) {
            r += "|"s;
        };
        return move(r) + to_string(x);
    };
    cout << "\tSIZE            : " << myset.size() << endl;
    cout << "\tCONTENT         : " << accumulate(myset.cbegin(), myset.cend(), ""s, f) << endl;
    cout << "\tLOAD FACTOR     : " << myset.load_factor() << endl;
    cout << "\tMAX LOAD FACTOR : " << myset.max_load_factor() << endl;
    cout << "\tNB BUCKETS      : " << myset.bucket_count() << endl;
    for (int i = 0; i < myset.bucket_count(); ++i) {
        cout << "\t\t bucket#" << i << " contains " << myset.bucket_size(i) << " elements" << endl;
    }
}

void insert(unordered_set<int>& myset, int element) {
    cout << "++++++ inserting element : " << element << endl;
    myset.insert(element);
    display(myset);
}

int main(int argc, char** argv) {
    unordered_set<int> myset;
    cout << "État INITIAL du set : " << endl;
    display(myset);
    insert(myset, 42);
    insert(myset, 44);
    insert(myset, 944);
    insert(myset, 844);
    insert(myset, 745);
    insert(myset, 645);

    cout << R"RAWSTRING(
La POC montre le resizing dynamique du unordered_set, lorsqu'on atteint le seuil max_load_factor.

load_factor = nb_items / nb_buckets = nombre MOYEN d'items par bucket
NOTE : ce n'est qu'une MOYENNE : si on n'a pas de bol, tous les items seront dans le même bucket :-(

Le rehashing est fait automatiquement à chaque fois que le load_factor atteint un seul défini.

Dans ce cas, on double le nombre de buckets, et on rehashe (i.e. on re-répartit les items dans les nouveaux buckets)

Attention, il n'y a PAS de mécanisme de rehashing automatique si un bucket en particulier est très rempli.
Le seul rehashing automatique porte sur le remplissage MOYEN des buckets.

Que se passe-t-il quand on insère un élément ?
    on calcule le bucket de l'élément : bucket_number = hash(elem) % NB_OF_BUCKETS
    si le bucket est vide -> on y place l'élément
    si le bucket n'est pas vide -> stratégie de résolution de collision (a priori, ici, il s'agit de separate_chaining)

Que se passe-t-il quand le container grandit ?
    on alloue de la mémoire pour les N nouveaux buckets
    pour chaque élément du set actuel, on le rehashe compte-tenu du nouveau nombre de buckets
    (en effet, comme le nombre de bucket a changé, le modulo à utiliser change aussi)
    on place l'élément dans son nouveau bucket
    une fois que tous les éléments sont replacés, on désalloue la mémoire des anciens buckets

)RAWSTRING"
         << endl;
    return 0;
}
