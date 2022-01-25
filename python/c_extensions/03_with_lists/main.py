import pouet


def main() -> None:
    print(
        """
In this POC, the function implemented in C expects a list of ints, and doubles its elements.
Nominal case is tested, along with various pathological cases (empty list, wrong arguments, etc.)

Various thoughts after this POC :
    - As I'm a beginner, I'm not 100% certain that what I implemented was the 'proper' way to do.
    - Error management is VERY tedious : even for this simple POC, I must return NULL no less than FIVE times !
    - Maybe error management would be easier with a third-party tool like pybind11 ?
    - If I miscode my C function, I can get this error :
        SystemError: <built-in function double_the_ints> returned NULL without setting an error

"""
    )
    input_ints = [42, 55, -3, 19]
    output1 = pouet.double_the_ints(input_ints)
    assert output1 == [x * 2 for x in input_ints]
    print("OK, nominal case")

    output2 = pouet.double_the_ints([])
    assert output2 == []
    print("OK, empty list")

    # ERROR CASE 1 = passing more arguments than needed :
    try:
        pouet.double_the_ints(input_ints, "coucou", 42)
    except TypeError:
        print("OK, we got a TypeError (expected = 'function takes exactly 1 argument (3 given)')")

    # ERROR CASE 2 = passing proper number of argument, but with wrong type :
    try:
        pouet.double_the_ints("coucou")
    except TypeError:
        print("OK, we got a TypeError (expected = 'argument 1 must be list, not str')")

    # ERROR CASE 3 = properly passing a list, but of list of wrong elements :
    try:
        pouet.double_the_ints(["a", "b", "c"])
    except TypeError:
        print("OK, we got a TypeError (expected = 'an integer is required (got type str)')")

    # ERROR CASE 3 = properly passing a list, and some of them are wrong :
    try:
        pouet.double_the_ints([42, 43, "a"])
    except TypeError:
        print("OK, we got a TypeError (expected = 'an integer is required (got type str)')")


if __name__ == "__main__":
    main()
