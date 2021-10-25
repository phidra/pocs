#include <iostream>

struct Inspector {
    Inspector(int& i, int& j) {
        std::cout << "Construction with 'int& int&'" << std::endl;
        ++i;
        ++j;
        was_called_with_ref_ref = true;
    }
    Inspector(int const&, int& j) {
        std::cout << "Construction with 'int const& int&'" << std::endl;
        ++j;
        was_called_with_cref_ref = true;
    }
    Inspector(int&&, int& j) {
        std::cout << "Construction with 'int&& int&'" << std::endl;
        ++j;
        was_called_with_rref_ref = true;
    }
    Inspector(int& i, int const&) {
        std::cout << "Construction with 'int& int const&'" << std::endl;
        ++i;
        was_called_with_ref_cref = true;
    }
    Inspector(int const&, int const&) {
        std::cout << "Construction with 'int const& int const&'" << std::endl;
        was_called_with_cref_cref = true;
    }
    Inspector(int&&, int const&) {
        std::cout << "Construction with 'int&& int const&'" << std::endl;
        was_called_with_rref_cref = true;
    }
    Inspector(int& i, int&&) {
        std::cout << "Construction with 'int& int&&'" << std::endl;
        ++i;
        was_called_with_ref_rref = true;
    }
    Inspector(int const&, int&&) {
        std::cout << "Construction with 'int const& int&&'" << std::endl;
        was_called_with_cref_rref = true;
    }
    Inspector(int&&, int&&) {
        std::cout << "Construction with 'int&& int&&'" << std::endl;
        was_called_with_rref_rref = true;
    }

    bool was_called_with_ref_ref = false;
    bool was_called_with_cref_ref = false;
    bool was_called_with_rref_ref = false;
    bool was_called_with_ref_cref = false;
    bool was_called_with_cref_cref = false;
    bool was_called_with_rref_cref = false;
    bool was_called_with_ref_rref = false;
    bool was_called_with_cref_rref = false;
    bool was_called_with_rref_rref = false;
};

struct AnotherInspector {
    AnotherInspector(std::string& x1, int& x2) {
        x1.append("_pouet");
        ++x2;
        std::cout << "ANOTHER construction with 'string& int&'" << std::endl;
        was_called_with_ref_ref = true;
    }

    AnotherInspector(std::string const&, int& x2) {
        ++x2;
        std::cout << "ANOTHER construction with 'string const& int&'" << std::endl;
        was_called_with_cref_ref = true;
    }

    AnotherInspector(std::string& x1, int const&) {
        x1.append("_pouet");
        std::cout << "ANOTHER construction with 'string& int const&'" << std::endl;
        was_called_with_ref_cref = true;
    }

    AnotherInspector(std::string&&, int const&) {
        std::cout << "ANOTHER construction with 'string&& int const&'" << std::endl;
        was_called_with_rref_cref = true;
    }

    bool was_called_with_ref_ref = false;
    bool was_called_with_cref_ref = false;
    bool was_called_with_ref_cref = false;
    bool was_called_with_rref_cref = false;
};
