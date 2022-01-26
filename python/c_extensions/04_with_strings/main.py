import pouet


def main() -> None:
    print(
        """
In this POC, the function implemented in C expects a string, and returns a new string with upper text.
Nominal case is tested, along with empty input string.

"""
    )
    input_string = "Luke"
    output1 = pouet.upper_the_string(input_string)
    assert output1 == input_string.upper()
    print("OK, nominal case")

    output2 = pouet.upper_the_string("")
    assert output2 == ""
    print("OK, empty string")


if __name__ == "__main__":
    main()
