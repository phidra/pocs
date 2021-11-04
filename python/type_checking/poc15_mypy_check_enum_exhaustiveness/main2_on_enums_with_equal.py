#!/usr/bin/env python3

from enum import Enum, auto


class Color(Enum):
    GREEN = auto()
    RED = auto()
    BLUE = auto()


def analyze(x: Color) -> str:
    if x == Color.RED:
        return "this is red as blood"
    elif x == Color.GREEN:
        return "this is green as grass"
    elif x == Color.BLUE:
        return "this is blue as the sea"

    # question 2 = does mypy thinks that there is an error here bc None is returned ?
    # answer 2 = sadly, it does (which is a BAD thing !) because it fails to understand that we never reach this line


if __name__ == "__main__":
    print(
        """
    All these POCs are part of a same group of POCs, whose goal is to define an "enum of instances" where mypy
    will detect if we fail to handle all the possible instances.
    Said otherwise : we want mypy to enforce that we exhaustively handled all the possible cases in an "enum".

    (the runtime is quite useless, the interest is to see how mypy analyzes the present file, and how it
    reacts when we uncomment some of its content)

    KO> The present POC shows that mypy FAILS TO FULFILLS this goal on an enum, if we use '==' to handle the cases.
    Notably : mypy fails to understand that 'analyze' properly handles all the possible values of 'x'.

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
