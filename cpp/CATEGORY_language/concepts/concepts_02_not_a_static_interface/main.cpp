#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = que les concepts ne sont (malheureusment) pas des interfaces statiques

En effet, certes le compilateur vérifie bien que le concept est respectée...
... mais il ne garantit pas qu'on n'utilise QUE ce qui est contraint.

Dans la présente POC, 'produce_noise' utilise la méthode 'scream' qui n'est nullement requise
par le concept.

Du coup les concepts aident à expliciter les contraintes qu'on souhaite voir respectées...
... mais ils reposent sur la  vigilance du développeur pour s'assurer qu'on a bien exprimé dans
le concept toutes les contraintes nécessaires à la fonction utilisant le concept.

)DELIM";
    cout << endl;
}

template <typename T>
concept CanQuack = requires(T v) {
    { v.quack() } -> std::convertible_to<std::string>;
};

template <typename T>
void produce_noise(T animal) requires CanQuack<T> {
    cout << "This animals quacks by saying : " << animal.quack() << endl;
    // BOUM : ici, on utilise quelque chose sur T qui n'est pas contraint par CanQuack :
    // ça montre que CanQuack n'est pas une interface statique...
    cout << "This animals screams by saying : " << animal.scream() << endl;
}

struct CatThatPretendsToBeADuck {
    std::string quack() { return string{"MEO... I mean : KWAK !"}; }
    std::string scream() { return string{"!!!!!!!!!!!!!!!!!!!!!!!!!!!"}; }
};

int main(void) {
    print_poc_description();
    CatThatPretendsToBeADuck fake_duck;
    produce_noise(fake_duck);
}
