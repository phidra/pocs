import pybind11_stl_structures as pbss


def main() -> None:
    print(
        """
This POC is a pybind11 example that uses std::string and std::vector

To run POC :
    pip install pybind11
    pip install -e .
    python main.py

WHAT THIS POC DOES :
    C++ code  = defines a C++ function that reverses a string + prefix/suffix.
    C++ code  = defines a C++ function that computes vector diff.
    C++ code  = uses the PYBIND11_MODULE macro to make those functions callable by python.
    setup.py  = defines a PyBind11 C++ extension, named 'pybind11_stl_structures'.
    main.py   = imports the pybind11_stl_structures module, and uses its functions.
"""
    )

    # passing and getting a std::string :
    input_string = "luke"
    expected_string = "BEFORE_ekul_AFTER"
    output_string = pbss.special_reverse_string(input_string)
    assert output_string == expected_string
    print("GOT STRING : ", output_string)

    # passing and getting a std::vector :
    input_array = list(range(5))
    expected_array = [1, 1, 1, 1]
    output_array = pbss.compute_diff(input_array)
    assert output_array == expected_array
    print("GOT STRING : ", output_array)


if __name__ == "__main__":
    main()
