#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <variant>
#include <vector>

using namespace std;

template <class>
inline constexpr bool always_false_v = false;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = l'utilisation des std::variant.

On stocke des objets différents (int, string, Coords) dans un std::vector, via un std::variant<int, string, Coords>.

On itère sur le vector et on applique un visiteur à chaque variant :
    les ints sont affichés directement
    les strings sont convertis en int (via un index externe) puis affichés
    les Coords lèvent une exception
)DELIM";
    cout << endl;
}

struct Coords {
    double lat;
    double lng;
    inline string to_string() const {
        ostringstream oss;
        oss << "Coords(lat=" << lat << ", lng=" << lng << ")";
        return oss.str();
    }
};

using Point = variant<int, string, Coords>;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    // les différents variants qu'on va essayer de traiter de façon uniforme (= en les convertissant en id) :
    // à noter que le vector in-fine contient des objets de type différents (par le biais du variant)
    vector<Point> points{
        Point{10},
        Point{"Paris"},
        Point{"Berlin"},
        Point{Coords{2.222, 8.888}},
        Point{99},
        Point{"Inexisting city"},
        Point{1},
    };

    // un index qui sert à convertir les variants de type string en id :
    unordered_map<string, int> index;
    index.insert(make_pair("Paris", 42));
    index.insert(make_pair("Berlin", 43));

    // PREMIÈRE FAÇON DE FAIRE = une lambda qui gère tous les variants possibles avec des static-if :
    auto convert_variant_to_id = [&index](auto&& arg) -> int {
        using T = decay_t<decltype(arg)>;
        if constexpr (is_same_v<T, int>) {
            // cout << "int with value " << arg << '\n';
            return arg;  // when variant contains an int, no need for conversion
        } else if constexpr (is_same_v<T, string>) {
            // cout << "string with value " << quoted(arg) << '\n';
            return index.at(arg);  // when variant contains a string, use the index to convert it to int
        } else if constexpr (is_same_v<T, Coords>) {
            // cout << "Coords with value " << arg.to_string() << '\n';
            throw logic_error("not implemented yet");  // conversion from Coords to int not implemented yet
        } else
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
    };

    // DEUXIÈME FAÇON DE FAIRE = un classe qui overloade l'operator() pour tous les variants possibles :
    struct MyVisitor {
        MyVisitor(unordered_map<string, int> _index) : index{_index} {}
        int operator()(int arg) {
            return arg;  // when variant contains an int, no need for conversion
        }
        int operator()(string const& arg) {
            return index.at(arg);  // when variant contains a string, use the index to convert it to int
        }
        int operator()(Coords const&) {
            throw logic_error("not implemented yet");  // conversion from Coords to int not implemented yet
        }

        unordered_map<string, int> index;
    };

    // UTILISATION = façon 1 :
    std::cout << "\nFaçon 1 = avec une lambda qui gère tous les variants possibles avec des static-if :" << std::endl;
    for (auto the_variant : points) {
        try {
            auto point_id = visit(convert_variant_to_id, the_variant);
            cout << "\tPoint id is = " << point_id << endl;
        } catch (...) {
            cout << "\tERROR> point id could not be computed" << endl;
        }
    }

    // UTILISATION = façon 2 :
    std::cout << "\nFaçon 2 = avec une classe qui overloade l'operator() pour tous les variants possibles :"
              << std::endl;
    for (auto the_variant : points) {
        try {
            auto point_id = visit(MyVisitor{index}, the_variant);
            cout << "\tPoint id is = " << point_id << endl;
        } catch (...) {
            cout << "\tERROR> point id could not be computed" << endl;
        }
    }

    return 0;
}
