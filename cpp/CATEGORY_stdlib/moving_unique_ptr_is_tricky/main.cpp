#include <iostream>
#include <memory>
#include <vector>

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC :
    1 = passer un T&& à une fonction ne garantit PAS que le T sera mové dans la fonction !
    2 = conséquence sur les unique_ptr = si on veut être CERTAIN qu'une fonction prendra l'ownership d'un unique_ptr,
    il faut que cette fonction prenne le unique_ptr par copie (plutôt que par rvalue-reference)

cf. https://stackoverflow.com/questions/8114276/how-do-i-pass-a-unique-ptr-argument-to-a-constructor-or-a-function/8114913#8114913

POINT 1 = en résumé, avec une fonction avec cette interface :
    void my_function(MyObject&&);

Appelée de cette façon :
    MyObject obj;
    my_function(std::move(obj);
    // ^ la question est : après cet appel, quel est l'état de 'obj' ? A-t-il été mové dans la fonction ?

Réponse = après cet appel, on ne peut pas savoir si obj a ou non été mové dans my_function !


POINT 2 = Conséquence sur les unique_ptr :

- de façon contre-intuitive, si on veut transférer l'ownership d'un unique_ptr a une fonction...
- ...mieux vaut que la signature de la fonction prenne l'unique_ptr par copie plutôt que par rvalue-reference !

void my_function(std::unique_ptr<MyObject>   obj)  // GOOD
void my_function(std::unique_ptr<MyObject>&& obj)  // BAD

En effet, la première forme permet d'assurer que le unique_ptr passé en paramètre est mové dans la fonction.

(car comme le unique_ptr est copié, on fait appel à son move-constructor, ce qui garantit que le unique_ptr passé au
callsite est mové dans la fonction)

)DELIM";
    cout << endl;
}

bool has_destructor_been_called = false;

// MyObject = une structure qui gère un gros buffer :
struct MyObject {
    std::vector<std::string> big_buffer;

    MyObject(std::vector<std::string> buffer_) : big_buffer{buffer_} {}
    MyObject(MyObject&& other) : big_buffer(std::move(other.big_buffer)) {}
    MyObject(MyObject const& other) : big_buffer(other.big_buffer) {}
    MyObject& operator=(MyObject&& other) {
        MyObject temp{std::move(other)};
        using std::swap;
        swap(*this, temp);
        return *this;
    }

    MyObject& operator=(MyObject const& other) {
        MyObject temp{other};
        using std::swap;
        swap(*this, temp);
        return *this;
    }
    ~MyObject() {
        cout << "+++ DESTRUCTOR CALLED !!! +++" << endl;
        has_destructor_been_called = true;
    }
};

void un_objet_passé_comme_rvalue_reference_a_une_fonction_n_est_pas_obligatoirement_vidé() {
    // ici, on illuste un point contre-intuitif = ce n'est pas parce qu'une fonction prend un argument comme
    // value-reference que l'argument est obligatoirement "mové" dans la fonction :
    cout << "****** Passer un argument par rvalue-reference ne garantit pas de le mover dans la fonction" << endl;

    auto do_not_actually_sink = [](MyObject&&) {};  // ne prend pas vraiment l'ownership de obj (on ne "vide" pas obj)
    auto do_actually_sink = [](MyObject&& obj) { MyObject other{std::move(obj)}; };  // ici, on vide réellement obj

    // GIVEN un MyObject obj qui contient un buffer à 3 itmes :
    MyObject obj({"hoho", "haha", "hihi"});
    cout << "=== Taille du buffer STEP 0 = " << obj.big_buffer.size() << endl;

    // WHEN on passe un std::move(obj) en argument d'une fonction :
    cout << "=== Appel de   do_not_actually_sink(MyObject&&)" << endl;
    do_not_actually_sink(std::move(obj));

    // THEN obj n'a pas été "vidé" par la fonction :
    cout << "=== Taille du buffer STEP 1 = " << obj.big_buffer.size() << endl;
    cout << "    ^ alors même qu'on a appelé une fonction qui prend un MyObject&&, l'objet n'a PAS été vidé !" << endl;
    if (obj.big_buffer.size() != 3) {
        throw std::runtime_error("on attendait 3");
    }

    // NOTE : une (autre) fonction similaire avec EXACTEMENT la même interface pourra vider obj :
    do_actually_sink(std::move(obj));
    cout << "=== Taille du buffer STEP 2 = " << obj.big_buffer.size() << endl;
    cout << "    ^ ici, une autre fonction avec EXACTEMENT la même interface a vidé obj !" << endl;
    if (obj.big_buffer.size() != 0) {
        throw std::runtime_error("on attendait 0");
    }
}

void mieux_vaut_passer_un_unique_ptr_par_value_que_par_rvalue_reference() {
    // de façon contre-intuitive, si on veut véhicule la sémantique de "je prends l'ownership de ton unique_ptr, et je
    // te laisse avec un unique_ptr vide", mieux vaut que la fonction attendre le unique_ptr par value ! en effet, si
    // elle attend le unique_ptr par rvalue-reference, on ne sait pas si elle va réellement mover son contenu ou non.

    // cette fonction attend certes une rvalue-ref vers un unique_ptr... mais elle n'en fait rien :
    auto expects_rvalue_ref_to_unique_ptr_but_do_nothing = [](std::unique_ptr<MyObject>&&) {};

    // cette fonction ne fait rien non plus avec l'objet, mais comme on lui passe le unique_ptr par value, l'ownership
    // est transférée à la fonction (et derrière, comme elle n'en fait rien, en pratique, ça détruit l'objet) :
    auto expects_unique_ptr_as_value = [](std::unique_ptr<MyObject>) {};

    // GIVEN = un unique_ptr de MyObject
    auto obj = std::make_unique<MyObject>(std::vector<std::string>{"hoho", "haha", "hihi"});
    has_destructor_been_called = false;

    // WHEN = on passe le unique_ptr par rvalue-reference
    expects_rvalue_ref_to_unique_ptr_but_do_nothing(std::move(obj));

    // THEN = il se peut que le contenu du unique_ptr passé au callsite ne soit pas réellement mové !
    if (has_destructor_been_called == true) {
        throw std::runtime_error("on ne s'attendait pas à ce que le destructor soit appelé !");
    }

    // WHEN = on passe le unique_ptr par value :
    expects_unique_ptr_as_value(std::move(obj));

    // THEN = quoi qu'il arrive, le unique_ptr passé au callsite est mové
    if (has_destructor_been_called == false) {
        throw std::runtime_error("on s'attendait à voir le destructor appelé !");
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    // POC 1 :
    un_objet_passé_comme_rvalue_reference_a_une_fonction_n_est_pas_obligatoirement_vidé();

    // POC 2 :
    mieux_vaut_passer_un_unique_ptr_par_value_que_par_rvalue_reference();
}
