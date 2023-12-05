#include <iostream>
#include <vector>

#include "glaze/glaze.hpp"

namespace example {
struct person {
    std::string first_name{};
    std::string last_name{};
    uint32_t age{};
};
}  // namespace example

template <>
struct glz::meta<example::person> {
    using T = example::person;
    static constexpr auto value = object("first_name", &T::first_name, "last_name", &T::last_name, "age", &T::age);
};

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = un autre setup possible pour utiliser la lib de parsing JSON glaze.

Le setup est issu d'un copié-collé manuel des includes de la lib directement depuis le repo :
    git clone https://github.com/stephenberry/glaze.git /tmp/glaze
    mkdir glaze_dependency
    cp -R /tmp/glaze/include glaze_dependency/


Le code sample n'a pas changé par rapport à la POC 1, il est repris quasi à l'identique de leur example project :
    https://github.com/stephenberry/glaze_example
    //
)DELIM";
    std::cout << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    using namespace example;
    std::vector<person> directory;
    directory.emplace_back(person{"John", "Doe", 33});
    directory.emplace_back(person{"Alice", "Right", 22});

    std::string buffer{};
    glz::write_json(directory, buffer);

    std::cout << buffer << "\n\n";

    std::array<person, 2> another_directory;
    auto error_code = glz::read_json(another_directory, buffer);

    if (error_code != glz::error_code::none) {
        std::cout << "Error reading buffer !" << std::endl;
        return 1;
    }

    std::string another_buffer{};
    glz::write_json(another_directory, another_buffer);

    if (buffer == another_buffer) {
        std::cout << "Directories are the same!\n";
    }

    return 0;
}
