#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // needed to convert python list to std::vector
#include <vector>
#include <string>

std::vector<int> compute_diff_cpp(std::vector<unsigned> const& input_vector);
std::string special_reverse_string_cpp(std::string const& s);

namespace py = pybind11;

PYBIND11_MODULE(pybind11_stl_structures, mod) {
    mod.def("compute_diff", &compute_diff_cpp, "Compute adjacent difference in array");
    mod.def("special_reverse_string", &special_reverse_string_cpp, "Reverse a string, adding prefix and suffix");
}
