#include <iostream>
#include <vector>
#include <cstring>

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = juste le même code que celui donné par Patrice ROY, mais corrigé pour compiler :

    https://h-deb.clg.qc.ca/Sujets/Divers--cplusplus/Metaprogrammation.html

EDIT : je lui ai signalé, il a corrigé.

Le principe de la POC est assez proche de ma POC sur enable_if, on veut branche statiquement sur l'implémentation d'une
fonction en fonction de caractéristiques statiques sur le type.

Ici, on veut choisir entre deux implémentations de copy :

    - une implémentation efficace si le type vérifie les conditions nous permettant d'utiliser un simple memcpy
    - une autre implémentation plus lente si on doit copier chaque élément un à un

TL;DR : le principe est de :
    - définir deux versions de la fonction 'copy' (une implémentation efficace, l'autre lente)
    - chacune des deux versions a une SIGNATURE différente, acceptant un paramètre supplémentaire d'un type différent
    - (chaque version dispose de son type, qui ne sert qu'à ça : discriminer les appels)
    - la version publique de 'copy' n'a pas de paramètre supplémentaire, mais elle appelle l'une ou l'autre des versions
      en utilisant conditional_t pour choisir le type de l'argument supplémentaire en fonction de conditions

)DELIM";
    std::cout << std::endl;
}

namespace my {
// ...
class raw_copying {};
class per_element_copying {};
template <class T>
using iter_categ_t = typename std::iterator_traits<T>::iterator_category;
template <class T>
using iter_value_t = typename std::iterator_traits<T>::value_type;
template <class T>
using remove_const_t = typename std::remove_const<T>::type;
template <class InIt, class OutIt>
void copy(InIt debutSrc, InIt finSrc, OutIt debutDest, raw_copying) {
    std::cout << "---> RAW" << std::endl;
    std::memcpy(reinterpret_cast<char*>(debutDest), reinterpret_cast<const char*>(debutSrc),
                std::distance(debutSrc, finSrc));  // dans ce cas, distance() est O(1)
}
template <class InIt, class OutIt>
void copy(InIt debutSrc, InIt finSrc, OutIt debutDest, per_element_copying) {
    std::cout << "---> PER ELEMENT" << std::endl;
    for (; debutSrc != finSrc; ++debutSrc, ++debutDest)
        *debutDest = *debutSrc;
}
template <class InIt, class OutIt>
void copy(InIt debutSrc, InIt finSrc, OutIt debutDest) {
    copy(debutSrc, finSrc, debutDest,
         std::conditional_t < std::is_same<iter_categ_t<InIt>, std::random_access_iterator_tag>::value &&
             std::is_same<iter_categ_t<OutIt>, std::random_access_iterator_tag>::value &&
             std::is_same<remove_const_t<iter_value_t<InIt>>, char>::value &&
             std::is_same<remove_const_t<iter_value_t<OutIt>>, char>::value,
         raw_copying, per_element_copying > {});
}
// ...
}  // namespace my

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    std::vector<std::string> src_per_element{"a", "b", "c", "z"};
    std::vector<std::string> dst_per_element{};
    dst_per_element.reserve(4);
    my::copy(src_per_element.begin(), src_per_element.end(), dst_per_element.begin());
    std::cout << "dst[1] = " << dst_per_element[1] << std::endl;
    std::cout << std::endl;

    char src_raw[4] = {'A', 'B', 'C', 'Z'};
    char dst_raw[4];
    my::copy(src_raw, src_raw + 4, dst_raw);
    std::cout << "dst[1] = " << dst_raw[1] << std::endl;
    std::cout << std::endl;
}
