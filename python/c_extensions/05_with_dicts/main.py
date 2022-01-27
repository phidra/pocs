import pouet


def main() -> None:
    print(
        """
In this POC, the function implemented in C expects a dict, and mutates it to delete/edit/add keys.
Nominal case is tested, along with empty input string.

Another functions creates a dict.

"""
    )
    input_dict = {
        "luke": "jedi",
        "luke_age": 42,
        "luke_sister": "leia",
    }
    output1 = pouet.edit_the_dict(input_dict)
    # "luke_age" is incremented
    # "luke_sister" is deleted
    # "luke_father" is added
    assert output1 == {
        "luke": "jedi",
        "luke_age": 43,
        "luke_father": "anakin",
    }
    print("OK, nominal case")

    pouet.edit_the_dict({"luke_age": 42, "luke_sister": "leia"})  # minimal ok case
    print("OK, minimal nominal case")

    # ERROR CASE 1 = key 'luke_age' is missing :
    try:
        pouet.edit_the_dict({"luke_sister": "leia"})
    except ValueError:
        print("OK, we got a ValueError (expected = Missing key 'luke_age')")

    # ERROR CASE 2 = key 'luke_sister' is missing :
    try:
        pouet.edit_the_dict({"luke_age": 42})
    except KeyError:
        print("OK, we got a KeyError (expected = 'luke_sister')")

    # ERROR CASE 3 = key 'luke_age' contains a string instead of an int :
    try:
        pouet.edit_the_dict({"luke_age": "coucou", "luke_sister": "leia"})
    except TypeError:
        print("OK, we got a TypeError (expected = an integer is required (got type str))")

    created = pouet.create_a_dict()
    assert created == {"star": "wars", "eleven squared": 121}
    print("OK, dict creation")


if __name__ == "__main__":
    main()
