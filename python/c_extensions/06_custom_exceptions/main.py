import pouet
from pouet import TeuopError


def main() -> None:
    print(
        """
This POC shows a basic C++ extension module for python, that uses custom exceptions.

In this POC, the function implemented in C raises a ValueError.
Moreover, it defines a custom exception and raises it.

"""
    )
    try:
        pouet.fail_with_ValueError()
    except ValueError as e:
        assert str(e) == "I am a dreadful ValueError, fear me !"
        print("OK, we got precisely the expected error")

    try:
        pouet.fail_with_PouetError()
    except TeuopError as e:
        assert str(e) == "I am a not less dreadful custom error !"
        print("OK, we got precisely the expected error")


if __name__ == "__main__":
    main()
