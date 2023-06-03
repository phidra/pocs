#include <iostream>
#include <optional>

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = utilisation basique des std::optional (à partir de C++17) :

- construction avec une valeur ou vide
- conditional si l'optional a une valeur ou bien est vide
- utilsation de la valeur
- destruction de l'objet = reset

cf. https://en.cppreference.com/w/cpp/utility/optional

)DELIM";
    cout << endl;
}

// if value is even, return the half of the value ; otherwise, returns an empty optional
std::optional<unsigned int> halve_if_even(unsigned int value) {
    cout << "About to halve : " << value << endl;
    if (value % 2 == 0) {
        return std::make_optional<unsigned int>(value / 2);
    } else {
        return std::optional<unsigned int>();
    }
}

void display_and_destroy(std::optional<unsigned int> value) {
    if (value.has_value()) {
        cout << "Halved value is : " << *value << endl;
    } else {
        cout << "Halved value is empty !" << endl;
    }
    value.reset();
    cout << endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    display_and_destroy(halve_if_even(42));
    display_and_destroy(halve_if_even(43));
}
