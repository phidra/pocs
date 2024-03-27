#include <functional>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

// std::bind est un peu l'équivalent de functools.partial en python
// ça renvoie un objet qui se comporte comme la fonction appelée avec certains arguments
// Il y a des subtilités sur le type des paramètres bindé, cf.
// https://fr.cppreference.com/w/cpp/utility/functional/bind
// https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
// https://fr.cppreference.com/w/cpp/utility/functional/placeholders

// EXEMPLE D'USAGE 1 = simplifier l'interface d'une fonction en figeant un argument :
void log(string const& level, string const& msg) {
    cout << "[" << level << "] " << msg << endl;
}
auto log_info = std::bind(log, "INFO", std::placeholders::_1);
auto log_error = std::bind(log, "ERROR", std::placeholders::_1);

// EXEMPLE D'USAGE 2 = réordonner les arguments :
void display_country(float longitude, float latitude) {
    string const& country = "FRANCE";  // stub
    cout << "(long=" << longitude << ", lat=" << latitude << ") is in... " << country << endl;
}

auto display_country_bound = std::bind(display_country, std::placeholders::_2, std::placeholders::_1);

int main(void) {
    // USAGE 1 = simplifier l'interface :
    cout << "On utilise log directement :" << endl;
    log("ERROR", "ceci est un message d'erreur");
    log("INFO", "ceci est un message d'info");
    cout << endl;

    cout << "On utilise les fonctions bindées log_info/log_error :" << endl;
    log_error("ceci est un autre message d'erreur");
    log_info("ceci est un autre message d'info");
    cout << endl;

    // USAGE 2 = réordonner les arguments :
    // Paris :
    // lng=2.3634
    // lat=48.8508
    cout << endl;
    cout << "Avec display_country qui accepte un latlong :" << endl;
    display_country(2.3634, 48.8508);

    cout << endl;
    cout << "Avec display_country_bound qui accepte un lonlat :" << endl;
    display_country_bound(48.8508, 2.3634);

    return 0;
}
