#!/usr/bin/env python3


def hello() -> str:
    print("HELLO WORLD")
    return "42"  # the returned value is a STRING


# but this function expects an INTEGER !
def print_returncode(returncode: int) -> None:
    res = returncode + 1000
    print(f"returncode + 1000 = {res}")


# the only relevant difference with poc.py is the annotation of the function 'main'
# which allow mypy to run and detect the bug :
def main() -> None:
    returncode = hello()
    print_returncode(returncode)


if __name__ == "__main__":
    main()
