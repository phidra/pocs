#include <vector>
#include <fstream>
#include <string>

#include <routingkit/contraction_hierarchy.h>

// returns the written file
template <typename Integer>
std::string dump_vector(std::vector<Integer> const& array, std::string const& filepath) {
    std::ofstream out(filepath);
    for (auto integer: array) {
        out << integer << "\n";
    }
    return filepath;
}

// returns the written file
std::string dump_bitvector(RoutingKit::BitVector const& bitvector, size_t nb_items, std::string const& filepath) {
    std::ofstream out(filepath);
    for (size_t index = 0; index < nb_items; ++index) {
        bool value = bitvector.is_set(index);
        out << value << "\n";
    }
    return filepath;
}


// returns the list of the written files
std::vector<std::string> dump_side(RoutingKit::ContractionHierarchy::Side const& side, std::string const& base_name) {
    std::vector<std::string> files;

    files.push_back(dump_vector(side.first_out, base_name + "_first_out.txt"));
    files.push_back(dump_vector(side.head, base_name + "_head.txt"));
    files.push_back(dump_vector(side.weight, base_name + "_weight.txt"));
    files.push_back(dump_vector(side.shortcut_first_arc, base_name + "_shortcut_first_arc.txt"));
    files.push_back(dump_vector(side.shortcut_second_arc, base_name + "_shortcut_second_arc.txt"));
    auto nb_arcs = side.head.size();
    files.push_back(dump_bitvector(side.is_shortcut_an_original_arc, nb_arcs, base_name + "_is_shortcut_an_original_arc.txt"));

    return files;
}
