#!/usr/bin/env python3

from enum import Enum, auto


class Color(Enum):
    GREEN = auto()
    RED = auto()
    BLUE = auto()
    # question 1 : if we ever uncomment BLACK below, does mypy detect that it's unhandled in analyze ?
    # answer 1 : it does (mypy detects an error), but that is a bit accidental (by luck), see another POC about this.
    # BLACK = auto()


def analyze(x: Color) -> str:
    if x is Color.RED:
        return "this is red as blood"
    elif x is Color.GREEN:
        return "this is green as grass"
    elif x is Color.BLUE:
        return "this is blue as the sea"

    # question 2 = does mypy thinks that there is an error here bc None is returned ? (when BLACK is commented above)
    # answer 2 = it does not (which is a good thing !) because it can deduct that we never reach this line


if __name__ == "__main__":
    print(
        """
    All these POCs are part of a same group of POCs, whose goal is to define an "enum of instances" where mypy
    will detect if we fail to handle all the possible instances.
    Said otherwise : we want mypy to enforce that we exhaustively handled all the possible cases in an "enum".

    (the runtime is quite useless, the interest is to see how mypy analyzes the present file, and how it
    reacts when we uncomment some of its content)

    OK> The present POC shows that mypy PERFECTLY FULFILLS this goal on an enum if we use 'is' to handle the cases.
    Notably : mypy properly understands that 'analyze' handles all the possible values of 'x'.
    Moreover, if we add a new value (BLACK) to the enum, mypy detects that we have to update 'analyze'.

    See also:
        https://github.com/python/mypy/issues/6366#issuecomment-673876383
        https://github.com/python/mypy/issues/5818
    """
    )

print(analyze(Color.RED))
print(analyze(Color.GREEN))

# question 3 = if we ever uncomment the line below, does mypy raise an error ?
# answer 3 = it does, which is a good thing
# print(analyze(42))
