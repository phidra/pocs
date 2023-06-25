#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;


// POC d'utilisation de std::remove_if
// TL;DR : ça move les éléments non-supprimés en tête du container, et ça renvoie un itérateur
//         indiquant où commencent les éléments à ne pas garder.
//         On peut passer un prédicat indiquant les éléments à supprimer.

// precondition = right is non-zero
bool is_approx_equal(int left, int right, float tolerance)
{
    float ratio = float(max(left, right)) / float(min(left, right));
    return ratio < 1.0 + tolerance;
}

vector<int> get_test_vector() { return {1, 35, 37, 38, 39, 24, 42, 44, 61, 42, 100, 42, 43, 40}; }

int main(void)
{
    // on ne veut garder que les éléments suffisamment éloignés de notre pivot :
    constexpr const int pivot = 42;

    // original vector :
    vector<int> vec1 = get_test_vector();
    cout << "INPUT DATA :" << endl;
    for (auto const& item: vec1) { cout << item << "\n"; }
    cout << endl;

    // test 1 = avec une lambda comparant directement :
    auto past_the_end_1 = remove_if(vec1.begin(), vec1.end(), [](int x){ return max(x, pivot) - min(x, pivot) < 6;});
    cout << "AFTER LAMBDA REMOVE_IF :" << endl;
    for (auto i = vec1.begin(); i != past_the_end_1; ++i) { cout << *i << "\n"; }

    // test 2 = avec un comparateur que je std::bind :
    vector<int> vec2 = get_test_vector();
    auto remove_checker = bind(is_approx_equal, pivot, placeholders::_1, 0.2);
    auto past_the_end_2 = remove_if(vec2.begin(), vec2.end(), remove_checker);
    cout << "AFTER BIND REMOVE_IF :" << endl;
    for (auto i = vec2.begin(); i != past_the_end_2; ++i) { cout << *i << "\n"; }

    return 0;
}
