import pouet


def main() -> None:
    print(
        """
This POC shows a basic C++ extension module for python, that uses dicts.

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
    except ValueError as e:
        assert str(e) == "Missing key 'luke_age'"
        print("OK, we got precisely the expected error")

    # ERROR CASE 2 = key 'luke_sister' is missing :
    try:
        pouet.edit_the_dict({"luke_age": 42})
    except KeyError as e:
        assert str(e) == "'luke_sister'"
        print("OK, we got precisely the expected error")

    # ERROR CASE 3 = key 'luke_age' contains a string instead of an int :
    try:
        pouet.edit_the_dict({"luke_age": "coucou", "luke_sister": "leia"})
    except TypeError as e:
        assert str(e) == "an integer is required (got type str)"
        print("OK, we got precisely the expected error")

    created = pouet.create_a_dict()
    assert created == {"star": "wars", "eleven squared": 121}
    print("OK, dict creation")


if __name__ == "__main__":
    main()
