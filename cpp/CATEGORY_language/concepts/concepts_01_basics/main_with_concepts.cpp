#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

template <typename T>
concept CanQuack = requires(T v) {
    { v.quack() } -> std::convertible_to<std::string>;
};

template <typename T>
void produce_noise(T animal) requires CanQuack<T> {
    cout << "This animals quacks by saying : " << animal.quack() << endl;
}

// not usable as an argument to 'produce_noise" :
struct Cat {
    std::string meow() const { return string{"MEOW !"}; }
};

struct CatThatPretendsToBeADuck : public Cat {
    std::string quack() { return string{"MEO... I mean : KWAK !"}; }
};

int main(void) {
    CatThatPretendsToBeADuck fake_duck;
    produce_noise(fake_duck);

    // this will not compile if uncommented :
    /* Cat cat; */
    /* produce_noise(cat); */
}
