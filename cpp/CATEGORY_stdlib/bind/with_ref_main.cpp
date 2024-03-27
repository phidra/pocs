#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

int DISPLAY_ON_COUT = 0;  // switch to enable/disable display on cout

// TL;DR = montre que si je bind ref(big_pivot), le pivot est bien passé à la fonction bindée par référence.

struct BigObject {
    int _value;
    BigObject() {
        if (DISPLAY_ON_COUT)
            cout << "DEFAULT CONSTRUCTION" << endl;
    }
    BigObject(int value) : _value{value} {
        if (DISPLAY_ON_COUT)
            cout << "CONVERSION CONSTRUCTION" << endl;
    }
    BigObject(BigObject& x) : _value{x._value} {
        if (DISPLAY_ON_COUT)
            cout << "COPY CONSTRUCTION" << endl;
    }
    BigObject(BigObject const& x) : _value{x._value} {
        if (DISPLAY_ON_COUT)
            cout << "COPY CONSTRUCTION 2" << endl;
    }
    BigObject(BigObject&& x) : _value{x._value} {
        if (DISPLAY_ON_COUT)
            cout << "MOVE CONSTRUCTION" << endl;
    }
    BigObject operator=(BigObject const& x) {
        if (DISPLAY_ON_COUT)
            cout << "COPY ASSIGNMENT" << endl;
        _value = x._value;
    }
    BigObject operator=(BigObject&& x) {
        if (DISPLAY_ON_COUT)
            cout << "MOVE ASSIGNMENT" << endl;
        _value = x._value;
    }
    ~BigObject() {
        if (DISPLAY_ON_COUT)
            cout << "DESTRUCTION" << endl;
    }

    friend ostream& operator<<(ostream& stream, BigObject const&);
};

ostream& operator<<(ostream& stream, BigObject const& x) {
    stream << x._value;
    return stream;
}

// precondition = right is non-zero
bool is_approx_equal(BigObject const& left, BigObject const& right, float tolerance) {
    float ratio = float(max(left._value, right._value)) / float(min(left._value, right._value));
    return ratio < 1.0 + tolerance;
}

vector<BigObject> get_test_vector() {
    return {1, 35, 37, 38, 39, 24, 42, 44, 61, 42, 100, 42, 43, 40};
}

int main(void) {
    // on ne veut garder que les éléments suffisamment éloignés de notre pivot :
    constexpr const int pivot = 42;
    DISPLAY_ON_COUT = 0;
    BigObject big_pivot(pivot);
    DISPLAY_ON_COUT = 1;

    // original vector :
    DISPLAY_ON_COUT = 0;
    vector<BigObject> vec1 = get_test_vector();
    DISPLAY_ON_COUT = 1;

    cout << "+++++ INPUT DATA +++++" << endl;
    for (auto const& item : vec1) {
        cout << item << "\n";
    }
    cout << endl;

    // test 1 = avec une lambda (prenant une référence) et comparant directement :
    DISPLAY_ON_COUT = 1;
    auto past_the_end_1 = remove_if(vec1.begin(), vec1.end(),
                                    [](BigObject& x) { return max(x._value, pivot) - min(x._value, pivot) < 6; });
    // comportement observé :
    //      - comme la lambda prend une référence, on ne voit pas de construction d'objet
    //      - (mais si on modifie la lambda pour accepter une copie, on voit N constructions par copie + destructions)
    //      - sur 5 éléments, les deux premiers sont conservés inchangés, les 3 suivants sont remplacés par d'autres
    //      (move-assignment)
    //      - pas clair : on voit autant de destruction que de move assignment, mais je ne vois pas les constructions à
    //      mettre en face...
    cout << "+++++ AFTER LAMBDA REMOVE_IF +++++" << endl;
    for (auto i = vec1.begin(); i != past_the_end_1; ++i) {
        cout << *i << "\n";
    }

    // test 2 = avec un comparateur que je std::bind :
    DISPLAY_ON_COUT = 0;
    vector<BigObject> vec2 = get_test_vector();
    DISPLAY_ON_COUT = 1;
    auto remove_checker = bind(is_approx_equal, cref(big_pivot), placeholders::_1, 0.2);
    auto past_the_end_2 = remove_if(vec2.begin(), vec2.end(), remove_checker);
    // comportement observé :
    //      - comme bind accepte un ref(big_pivot), on ne voit pas de construction d'objet
    //      - (j'en déduis que le placeholder est correctement passé par référence, sans que j'aie à passer
    //      ref(placeholders::_1) )
    //      - si je passe un big_pivot au lieu d'un ref(big_pivot), je vois plusieurs copy-constructions, corresondan
    //      aux constructions de big_pivot
    //      - pas clair : pourquoi je ne vois pas autant de copy-constructions que d'items dans mon INPUT DATA ?
    //      - par ailleurs, j'observe le même comportement (move-assignement + destruction) qu'avec le test précédent
    cout << "+++++ AFTER BIND REMOVE_IF +++++" << endl;
    for (auto i = vec2.begin(); i != past_the_end_2; ++i) {
        cout << *i << "\n";
    }

    // Don't display all the destructions :
    DISPLAY_ON_COUT = 0;

    return 0;
}
