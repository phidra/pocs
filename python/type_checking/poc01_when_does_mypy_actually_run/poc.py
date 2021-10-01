#!/usr/bin/env python3


def hello() -> str:
    print("HELLO WORLD")
    return "42"  # the returned value is a STRING


# but this function expects an INTEGER !
def print_returncode(returncode: int) -> None:
    res = returncode + 1000
    print(f"returncode + 1000 = {res}")


def main():
    returncode = hello()

    # so there is a bug here : we pass a STRING returned value to a function that expects an INTEGER
    # When run, this code will THROW, and we expect the type-checker to find this bug...
    print_returncode(returncode)

    # Will the type-checker catch this bug ? It depends...
    # By default, mypy doesn't check unannotated function, and "main" is not annotated -> the bug will not be caught
    # solution 1 = use the flag --check-untyped-defs :
    #    https://mypy.readthedocs.io/en/stable/command_line.html#cmdoption-mypy-check-untyped-defs
    # solution 2 = annotate the function


if __name__ == "__main__":
    main()
