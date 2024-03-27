#include <iostream>
#include <unordered_set>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

void iteration_through_buckets(unordered_set<int> const& myset) {
    cout << "ITERATION THROUGH BUCKETS :" << endl;
    for (int bucket = 0; bucket < myset.bucket_count(); ++bucket) {
        for (auto elemIt = myset.cbegin(bucket); elemIt != myset.cend(bucket); ++elemIt) {
            cout << "\t element : " << *elemIt << "   -> bucket# " << bucket << endl;
        }
    }
}

size_t get_bucket(int value, unordered_set<int> const& myset) {
    for (int bucket = 0; bucket < myset.bucket_count(); ++bucket) {
        for (auto elemIt = myset.cbegin(bucket); elemIt != myset.cend(bucket); ++elemIt) {
            if (*elemIt == value) {
                return bucket;
            }
        }
    }
    throw std::logic_error("unexistent value");
}

void direct_iteration(unordered_set<int> const& myset) {
    cout << "DIRECT ITERATION : " << endl;
    for (auto x : myset) {
        cout << "\t element : " << x << "   -> bucket# " << get_bucket(x, myset) << endl;
    }
}

int main(int argc, char** argv) {
    unordered_set<int> myset;
    myset.reserve(8);
    /* auto v = vector<int>(8); */
    /* std::iota(v.begin(), v.end(), 1);  // AVEC ces valeurs, chaque élément est dans un bucket différent -> j'itère
     * bien dans l'ordre inverse de l'ajout, ce qui est cohérent avec le source-code */
    auto v = vector<int>({239, 245, 644, 844, 244, 346, 344, 544});
    for (auto x : v) {
        cout << "+++ inserting " << x << endl;
        myset.insert(x);
    }
    direct_iteration(myset);
    iteration_through_buckets(myset);

    const char* poc_description = R"RAWSTRING(
La POC était supposée montrer que l'itération d'un set itérait sur ses buckets...

En pratique, elle montre que ça n'EST PAS le cas :-)

EDIT : et c'est tant mieux, sinon pour un set de 1000 buckets ne contenant que 2  éléments, on itèrerait 1000 fois au lieu de 2...

Du coup, je ne sais pas comment sont implémentés les itérateurs d'unordered_set, voyons ce qu'en dit le code :


cbegin renvoie __table_.begin()
    https://github.com/llvm-mirror/libcxx/blob/3f39fe31c45059098e8d3cb84b74f8bfe3191e5e/include/unordered_set#L529

__table_ est une __table, a.k.a une __hash_table
    https://github.com/llvm-mirror/libcxx/blob/3f39fe31c45059098e8d3cb84b74f8bfe3191e5e/include/unordered_set#L397

__hash_table.begin() renvoie __p1.first().__next_
    https://github.com/llvm-mirror/libcxx/blob/3c73561841650afb4718223958b4b6e86983c862/include/__hash_table#L1799

__p1 est une paire dont le premier élément est un __first_node :
    https://github.com/llvm-mirror/libcxx/blob/3c73561841650afb4718223958b4b6e86983c862/include/__hash_table#L955

En pratique, je ne vois pas comment est initialisé ce __p1 ?

J'ai peut-être une piste, quand je regarde comment les éléments sont insérés dans le set :

je commence avec le "insert" de l'unordered_set :

in fine, j'atteris ici :
    https://github.com/llvm-mirror/libcxx/blob/3c73561841650afb4718223958b4b6e86983c862/include/__hash_table#L1905

on récupère le numéro de bucket dans lequel on va insérer l'élément :
    https://github.com/llvm-mirror/libcxx/blob/3c73561841650afb4718223958b4b6e86983c862/include/__hash_table#L1909

https://github.com/llvm-mirror/libcxx/blob/3c73561841650afb4718223958b4b6e86983c862/include/__hash_table#L1911
    on récupère le contenu du bucket : nullptr si le bucket est vide, un node existant sinon


https://github.com/llvm-mirror/libcxx/blob/3c73561841650afb4718223958b4b6e86983c862/include/__hash_table#L1922
    c'est le cas où le bucket contenait déjà un noeud :
        __nd est le nouveau noeud inséré
        __pn est le noeud existant dans le bucket concerné
        NODE.__next pointe vers le noeud suivant
        NODE.__ptr pointe (supposément) vers le contenu du noeud
    __pn va rester le premier noeud du bucket
    en revanche, on fait pointer __pn vers __nd (qui devient donc le second noeud)
    et du coup, on fait pointer __nd vers l'ancien second noeud
    TL;DR : on insère le nouveau noeud juste après le premier noeud du bucket

https://github.com/llvm-mirror/libcxx/blob/3c73561841650afb4718223958b4b6e86983c862/include/__hash_table#L1914
    TL;DR : on insère le nouveau noeud juste après le premier noeud du set
    le nouveau noeud pointe maintenant vers l'ancien second noeud

https://github.com/llvm-mirror/libcxx/blob/3c73561841650afb4718223958b4b6e86983c862/include/__hash_table#L1918
    on n'oublie pas de faire en sorte que le bucket[N] pointe vers le noeud ajouté

du coup, on maintient des liens entres les noeuds (le set est en fait également une liste chaînée !)
quand on parle de "premier noeud" ou second noeud, il y a donc deux notions :
    vis-à-vis de la liste chaînée permettant d'itérer sur tous les noeuds du set
    vis-à-vis de la liste chaînée interne à un bucket
(a priori c'est la même liste chaînée, donc je ne sais pas comment il s'arrête)
(EDIT : il a l'air de s'arrêter lorsque le chash de l'élément suivant n'est pas le même que le chash de l'élément précédent)


J'avoue ne pas comprendre pourquoi on a besoin de ça :
    https://github.com/llvm-mirror/libcxx/blob/3c73561841650afb4718223958b4b6e86983c862/include/__hash_table#L1919
ça met à jour le bucket de l'ancien second noeud, mais pour moi celui-ci n'a pas besoin d'être mis à jour, vu qu'il n'a pas été modifié...


Le code du erase est instructif aussi :
https://github.com/llvm-mirror/libcxx/blob/3c73561841650afb4718223958b4b6e86983c862/include/__hash_table#L2661

Notamment, c'est sans doute en calculant le chash des éléments que je sais que j'ai fini d'itérer sur un bucket
(en effet, quand le chash de l'élément suivant n'est pas le même que le chash de l'élément précédent, alors 
 l'élément précédent est le dernier élément du buket)
note : chash est l'index du bucket dans lequel on place un node

NOTE : le fait que begin renvoie __p1_.first().__next_ me laisse à penser que __p1.ptr() a une signification particulière QUI N'EST PAS celle d'un node :
    https://github.com/llvm-mirror/libcxx/blob/3c73561841650afb4718223958b4b6e86983c862/include/__hash_table#L1799
    (en gros, on n'itère pas dessus)

NOTE : voir aussi : https://stackoverflow.com/questions/31112852/how-stdunordered-map-is-implemented
    (et voir la vidéo référencée)

)RAWSTRING";
    /* cout << poc_description << endl; */
    return 0;
}
