#include <functional>
#include <iostream>

using namespace std;

/*
   https://www.nextptr.com/tutorial/ta1441164581/stdref-and-stdreference_wrapper-common-use-cases

   std::ref<T> est converti implicitement en un T&
   (et encapsule un pointeur, en vrai, mais c'est du détail d'implémentation)
   l'un des trucs cools, c'est que le wrapper est copiable/assignable, donc plus souple à utiliser qu'une vraie
   référence

   CAS D'USAGE 1 = permet de passer plus facilement une référence comme paramètre template.
        SANS std::ref   tuple<UnTypeQuiPeutEtreTresComplexe&, int, UnAutreTypeTresComplexe&> t1(m, n, s);
        AVEC std::ref   auto t2 = make_tuple(ref(m), n, ref(s));

   CAS D'USAGE 2 = permet de faire un tableau/container de références (interdit sinon).
   CAS D'USAGE 3 = passer un argument par référence à la start-function d'un thread
   CAS D'USAGE 4 = avoir une référence comme membre de classe (car avec une "vraie" référence, c'est compliqué,
   notamment car la classe n'est plus assignable) CAS D'USAGE 5 = passer par référence un objet de type fonction CAS
   D'USAGE 6 = passer par référence à std::bind
*/

template <typename T>
void increment(T x) {
    x += 1;
}

template <typename T>
void assign_100_KO(T x) {
    x = 100;
}

template <typename T>
void assign_100_OK(T x) {
    x.get() = 100;
}

int main(int argc, char* argv[]) {
    // ===== CAS D'USAGE 1 = passer une référence comme paramètre template :

    // sans précaution, T=int et x N'EST PAS muté :
    int y1 = 42;
    int& x1 = y1;
    increment(x1);
    cout << "VERSION 1 :   x = " << x1 << endl;

    // avec passage explicite, T=int& et x est muté :
    int y2 = 42;
    int& x2 = y2;
    increment<int&>(x2);
    cout << "VERSION 2 :   x = " << x2 << endl;

    // avec std::ref, pas besoin du passage explicite, T=int& et x est tout de même muté :
    int y3 = 42;
    int& x3 = y3;
    increment(std::ref(x3));
    cout << "VERSION 3 :   x = " << x3 << endl;

    // attention que même si on peut muter, on n'a tout de même pas le droit d'ASSIGNER, à moins d'utiliser .get() :
    int y4 = 42;
    int& x4 = y4;
    // assign_100(std::ref(x4));  // ne compile pas
    assign_100_OK(std::ref(x4));
    cout << "VERSION 4 :   x = " << x4 << endl;

    // ===== CAS D'USAGE 2 = avoir un tableau de références :
    int a = 1, b = 2, c = 3;
    int &aref = a, &bref = b, &cref = c;
    // int& array[] = {aref, bref, cref};  // ne compile pas, on n'a pas le droit de faire un tableau de références
    std::reference_wrapper<int> array[] = {std::ref(a), std::ref(b), std::ref(c)};
    array[1] += 10;
    cout << "b = " << b << endl;  // 12

    return 0;
}
