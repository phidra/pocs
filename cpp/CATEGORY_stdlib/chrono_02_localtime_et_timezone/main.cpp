#include <string.h>

#include <chrono>
#include <iomanip>
#include <iostream>

using namespace std;

void print_poc_description() {
    cout << R"DELIM(
CONTEXTE = on veut afficher un timepoint (std::time_t) en datetime affichable

    - ATTENTION : cet affichage dépend de la timezone (heure locale + application ou non de l'heure d'hiver, etc.)
    - en effet, un même moment dans le temps, qui a lieu à 18h42 à Paris, s'exprime comme "à 10h42 à New-York"

CE QUE MONTRE CETTE POC :

    - l'affichage de base des datetimes avec :
        - std::gmtime pour exprimer le timepoint avec un datetime UTC
        - std::localtime pour exprimer le timepoint avec un datetime dans la timezone du système
    - le changement de la timezone que std::localtime utilise, grâce à setenv("TZ")


Dans les affichages sur cout, prêter une attention particulière à l'HEURE (ou au décalage par rapport à UTC), car c'est
celle-ci qui va changer selon la timezone utilisée.

À noter : modifier la timezone locale avec "TZ" n'impacte QUE le processus courant (la timezone système est inchangée)

)DELIM";
    cout << endl;
}

constexpr char const* FMT = "%Y-%m-%dT%H:%M:%S%z";

void set_timezone(std::string const& timezone) {
    int retcode = setenv("TZ", timezone.c_str(), 1);
    if (retcode < 0) {
        // D'après man 3 setenv, les seuls deux cas d'erreurs possibles sont :
        //     EINVAL name is NULL, points to a string of length 0, or contains an '=' character.
        //     ENOMEM Insufficient memory to add a new variable to the environment.
        std::string err{strerror(errno)};
        throw std::runtime_error(err);
    }

    tzset();
    // D'après man 3 tzset :
    // If the TZ variable does appear in the environment, but its value is empty, or its value cannot be > interpreted
    // using any of the formats specified below, then Coordinated Universal Time (UTC) is used.
    //
    // If the TZ variable does not appear in the environment, the system timezone is used. > The system timezone is
    // configured by copying, or linking, a file in the tzfile(5) format to /etc/localtime.
}

void unset_timezone() {
    // on peut revenir au comportement par défaut avec unsetenv (avec les mêmes cas d'erreurs que setenv) :
    int retcode = unsetenv("TZ");
    if (retcode < 0) {
        std::string err{strerror(errno)};
        throw std::runtime_error(err);
    }
    tzset();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    auto now_chrono = std::chrono::system_clock::now();
    std::time_t now = std::chrono::system_clock::to_time_t(now_chrono);
    std::cout << "Le timepoint (std::time_t) à exprimer comme datetime est :   now=" << now << std::endl;

    std::cout << std::endl;
    std::cout << "Comportement par défaut = on exprime now avec un datetime UTC ou en heure LOCALE :" << std::endl;
    std::cout << "\t" << std::put_time(std::gmtime(&now), FMT) << " in UTC" << std::endl;
    std::cout << "\t" << std::put_time(std::localtime(&now), FMT) << " in LOCAL timezone" << std::endl;

    std::cout << std::endl;
    std::cout << "Définir l'envvar TZ permet de changer la timezone utilisée par std::localtime :" << std::endl;
    auto display_time_at = [&now](std::string timezone) {
        set_timezone(timezone);
        std::cout << "\t" << std::put_time(std::localtime(&now), FMT) << " in '" << timezone << "'" << std::endl;
    };
    display_time_at("Asia/Tokyo");
    display_time_at("Europe/Paris");
    display_time_at("UTC");
    display_time_at("une timezone invalide sera interprétée comme UTC");
    display_time_at("America/Chicago");

    // retour au comportement par défaut :
    std::cout << std::endl;
    std::cout << "Sans l'envvar TZ, le comportement de std::localtime redevient celui par défaut :" << std::endl;
    std::cout << "\tAVANT unsetenv(\"TZ\");" << std::endl;
    std::cout << "\t" << std::put_time(std::localtime(&now), FMT) << " in LOCAL timezone" << std::endl;
    std::cout << "\tAPRÈS unsetenv(\"TZ\");" << std::endl;
    unset_timezone();
    std::cout << "\t" << std::put_time(std::localtime(&now), FMT) << " in LOCAL timezone" << std::endl;
}
