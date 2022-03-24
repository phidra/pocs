#include <iostream>
#include <string>

using namespace std;

struct Person {
    string first_name;
    string last_name;
};

struct AgedPerson {
    int age;
    Person* who;
};

void mutate_person_through_aged_person(AgedPerson const& aged_person) {
    aged_person.who->first_name = "Anakin";
}

template <typename T>
void assert_equal(T x, T expected) {
    if (x != expected) {
        throw std::runtime_error{"assertion failed"};
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    cout << R"DELIM(
Can we mutate an object pointed (by a non-const pointer) from a const object ?

As expected, yes we can : what we CANNOT do is reassign the pointer of a const object.

)DELIM";

    Person luke{"Luke", "Skywalker"};
    const AgedPerson aged_luke{42, &luke};  // this is const, but we'll still mutate its Person
    cout << "Before mutation : " << aged_luke.who->first_name << " " << aged_luke.who->last_name << endl;
    assert_equal(aged_luke.who->first_name, string("Luke"));
    assert_equal(aged_luke.who->last_name, string("Skywalker"));

    // mutating Person through the const AgedPerson is allowed :
    mutate_person_through_aged_person(aged_luke);
    cout << "After  mutation : " << aged_luke.who->first_name << " " << aged_luke.who->last_name << endl;
    assert_equal(aged_luke.who->first_name, string("Anakin"));
    assert_equal(aged_luke.who->last_name, string("Skywalker"));

    return 0;
}

/* // what is not allowed is to reassign AgedPerson's pointer (this won't compile if uncommented) : */
/* void mutate_person(AgedPerson const& aged_person, Person* new_person) { */
/*     aged_person.who = new_person; */
/* } */
