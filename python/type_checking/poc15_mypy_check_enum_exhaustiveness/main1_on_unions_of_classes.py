#!/usr/bin/env python3

from typing import Union


class Red:
    pass


class Green:
    pass


class Blue:
    pass


class Black:
    pass


Color = Union[
    Red,
    Green,
    Blue,
    # question 1 : if we ever uncomment Black below, does mypy detect that i's missing in analyze ?
    # answer 1 : it does (mypy detects an error), but that is a bit accidental (by luck), see another POC about this.
    # Black,
]


def analyze(x: Color) -> str:
    if isinstance(x, Red):
        return "this is red as blood"
    if isinstance(x, Green):
        return "this is green as grass"
    if isinstance(x, Blue):
        return "this is blue as the sea"

    # question 2 = does mypy thinks that there is an error here bc None is returned ? (when Black is commented above)
    # answer 2 = it does not (which is a good thing !) because it can deduct that we never reach this line


if __name__ == "__main__":
    print(
        """
    All these POCs are part of a same group of POCs, whose goal is to define an "enum of instances" where mypy
    will detect if we fail to handle all the possible instances.
    Said otherwise : we want mypy to enforce that we exhaustively handled all the possible cases in an "enum".

    (the runtime is quite useless, the interest is to see how mypy analyzes the present file, and how it
    reacts when we uncomment some of its content)

    OK> The present POC shows that mypy PERFECTLY FULFILLS this goal if our "enum" is a Union of classes.
    Notably : mypy properly understands that 'analyze' handles all the possible values of 'x'.
    """
    )

    print(analyze(Red()))
    print(analyze(Green()))

    # question 3 = if we ever uncomment the line below, does mypy raise an error ?
    # answer 3 = it does, which is a good thing
    # print(analyze(42))
