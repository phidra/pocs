#include <iostream>
#include <string>

using namespace std;

std::pair<int, int> get_ints() {
    return {1, 2};
}

// NOTE : le bug se présente avec clang++ (clang version 10.0.0-4ubuntu1)
//        le bug ne se présente PAS avec g++ (Ubuntu 9.3.0-17ubuntu1~20.04)

int main(void) {
    auto [a, b] = get_ints();

    // 'a', issue d'un structured bindings est tout à fait utilisable :
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    // pourtant, elle n'est pas capturable dans une lambda (cette ligne ne compile pas avec clang++) :
    auto display_a_KO = [&a]() { std::cout << "a by lambda = " << a << std::endl; };

    // contournement = aliaser 'a' :
    auto& a_bis = a;
    auto display_a_OK = [&a_bis]() { std::cout << "a by lambda = " << a_bis << std::endl; };

    return 0;
}
