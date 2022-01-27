import pouet


def main() -> None:
    print(
        """
This POC shows a basic C++ extension module for python.

It is the implementation of the example given in this doc, but in C++ :

https://docs.python.org/3/extending/extending.html

Compared to the previous POC, the only change is the extension of the file, and its use of C++.
(setup.py properly use a C++ compiler when module ends with .cpp)

"""
    )
    statement = "Luke Skywalker is a jedi"
    nb_chars = pouet.count_nb_chars(statement)
    assert nb_chars == len(statement)
    print(f"OK : statement with {nb_chars} chars was : '{statement}'")


if __name__ == "__main__":
    main()
