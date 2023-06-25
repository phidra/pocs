#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;
using std::find_if;
 
int main()
{
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto is_divisible_by_5 = [](int i){ return i % 5 == 0;};
    cout << "Les entiers de 1 à 12 contiennent un multiple de 5 ? " << (find_if(v.begin(), v.end(), is_divisible_by_5) != v.end()) << endl;
}
