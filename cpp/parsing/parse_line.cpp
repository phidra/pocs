#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <cassert>

using namespace std;

// Cette POC montre comment utiliser getline pour parser une ligne donnée en tokens.
// NOTE : je laisse de côté le cas tricky où l'un des token contient le delimiter (échappé).
//
// Les enseignements de la POC (en plus de démontrer le code de parsing) :
//
// Saut de ligne final :
//      en utilisant ',' comme délimiteur de getline comme le fait mon code, si la ligne
//      à parser comporte un saut de ligne (\n) final, elle se retrouvera dans le token.
//
// Validité d'un stream, et ligne vide :
//      un stream est ".good()" même lorsqu'il et construit sur une ligne vide
//      ce n'est que lorsqu'on essaye de le LIRE qu'un stream vide passe en ".bad()"
//      par conséquent, un getline sur une ligne vide va setter le stream à bad (et le token correspondant sera vide)
//      dans mon code de parsing ci-dessous, une ligne vide est donc convertie en un seul token vide (ce qui est le
//      comportement souhaité)
//
// Comportement de getline : caractères renvoyés + conditions d'invalidité du stream ?
//      doc = https://en.cppreference.com/w/cpp/string/basic_string/getline
//      La doc est assez claire sur quand on arrête de parser et quand le stream devient invalide :
//          getline(STREAM, STR, DELIM) extrait les caractères de STREAM, et les append à la STR jusqu'à ce que :
//              1. on épuise le stream (auquel cas getline sette eofbit)
//              2. on tombe sur DELIM (auquel cas getline retourne, mais on n'append PAS le DELIM à la STR)
//              3. on n'extrait aucun caractère (auquel cas getline sette failbit)
//      La fin de ligne de chacun des tests ci-dessous tombe dans le cas 1. ou le cas 3.
//
// Que se passe-t-il quand le dernier caractère est une virgule ?
//      getline parse token après token. Lorsqu'il arrive à la virgule (finale, mais getline ne le sait pas encore)...
//      ...il sette la string précédant la virgule dans le token, consomme la vigule, puis retourne
//      On repart alors pour l'itération suivante, mais comme la virgule était le dernier caractère de la ligne...
//      ... getline se retrouve immédiatement dans la situation où il n'y a plus de caractères à récupérer.
//      Du coup il ne mets rien dans la STR, il sette eofbit, et retourne.
//      (avec mon code de parsing ci-dessous, ça se traduit par le fait qu'on renvoie un dernier token vide, ce qui est
//      le comportement souhaité)

vector<string> split_line(string const& line) {
    // pour cette POC, le délimiteur est fixé :
    const char COMMA = ',';

    vector<string> tokens;
    stringstream ss(line);

    // NOTE : une raison pour laquelle ce code se comporte comme expected est que si getline ne trouve rien, il renvoie
    // une string vide.
    while (ss.good()) {
        string substr;
        getline(ss, substr, COMMA);
        tokens.push_back(substr);
    }
    return tokens;
}

bool check(string testId, string const& line, vector<string> const& expected_tokens) {
    cout << boolalpha;
    vector<string> tokens = split_line(line);
    cout << (tokens == expected_tokens ? "[OK]" : "BOUM") << "      " << testId << endl;
}

int main(void) {
    // au passage : je vérifie qu'un stream initialisé sur une ligne vide est good :
    stringstream emptystream("");
    assert(emptystream.good());
    char got = emptystream.get();
    // ce n'est que lorsqu'on essaye d'en récupérer le contenu que son état est setté à eofbit :
    assert(emptystream.eof());
    assert(!emptystream.good());  // un stream épuisé n'est plus prêt à être lu
    assert(emptystream.fail());   // un stream épuisé semble setter failbit (un peu curieux, vu sa définition)
    assert(!emptystream.bad());   // un stream épuisé ne sette pas badbit
    assert(got == EOF);

    // Ces tests indiquent le comportement souhaité dans chaque cas.
    // Notamment, dans plusieurs cas, on VEUT récupérer un token vide.
    check("basic", "padme,amidala,62,Coruscant", {"padme", "amidala", "62", "Coruscant"});
    check("trailing comma", "leia,skywalker,35,", {"leia", "skywalker", "35", ""});
    check("emptyline", "", {""});
    check("singletoken", "luke", {"luke"});
    check("three empty tokens", ",,", {"", "", ""});
    check("singletrailing", "luke,", {"luke", ""});
    check("singleleading", ",luke", {"", "luke"});
    check("withnewline", "leia,skywalker,35,\n", {"leia", "skywalker", "35", "\n"});
    check("withnewline2", "leia,skywalker,35\n", {"leia", "skywalker", "35\n"});
}
