#!/usr/bin/env python3

from typing import NoReturn, Tuple, Literal
from dataclasses import dataclass


@dataclass
class ColorClass:
    R: float
    G: float
    B: float

    def as_YUV(self) -> Tuple[float, float, float]:
        Y = 0.257 * self.R + 0.504 * self.G + 0.098 * self.B + 16
        U = -0.148 * self.R - 0.291 * self.G + 0.439 * self.B + 128
        V = 0.439 * self.R - 0.368 * self.G - 0.071 * self.B + 128
        return Y, U, V


RED = ColorClass(1, 0, 0)
GREEN = ColorClass(0, 1, 0)
BLUE = ColorClass(0, 0, 1)

# sadly, this is not possible (Literal only works with primitive values, and not custom objects :-(
# Color = Literal[
#     RED,
#     GREEN,
#     BLUE,
# ]

Color = Literal[
    "red",
    "green",
    "blue",
    # question 1 : if we ever uncomment black below, does mypy detect that i's missing in analyze ?
    # answer 1 : it does (mypy detects an error), but that is a bit accidental (by luck), see another POC about this.
    # "black",
]


def assert_that_this_line_is_never_reached(value: NoReturn) -> NoReturn:
    assert False, f"Unhandled value: {value} ({type(value).__name__})"


def analyze(x: Color) -> str:
    if x == "red":
        return "this is red as blood"
    if x == "green":
        return "this is green as grass"
    if x == "blue":
        return "this is blue as the sea"

    # question 2 = does mypy thinks that there is an error here bc None is returned ? (when black is commented above)
    # answer 2 = it does not (which is a good thing !) because it can deduct that we never reach this line
    assert_that_this_line_is_never_reached(x)


if __name__ == "__main__":
    print(
        """
    All these POCs are part of a same group of POCs, whose goal is to define an "enum of instances" where mypy
    will detect if we fail to handle all the possible instances.
    Said otherwise : we want mypy to enforce that we exhaustively handled all the possible cases in an "enum".

    (the runtime is quite useless, the interest is to see how mypy analyzes the present file, and how it
    reacts when we uncomment some of its content)

    NOT REALLY OK> this POC uses typing.Literal, the results are positive but too limited.
    +++ On the plus side, mypy warns if one case of the Literal is unhandled.
    +++ Moreover, it can deduce that the assert is properly unreachable, so the goal is attained.
    --- On the minus side, Literals are very limited : they can only be defined on primitive types, and
        not on custom objects.
    --- Moreover, even if a function expects Literal of strings, we are forced to pass it a literal, i.e. we
       cannot pass it a mere variable.

    Still, it could probably be useful on corner-cases.

    See also: https://mypy.readthedocs.io/en/stable/literal_types.html
    """
    )

    print(analyze("red"))
    print(analyze("green"))

    # wow, even passing a VARIABLE is forbidden with Literal (mypy complains if following lines are uncommented) :
    # my_super_green = "green"
    # print(analyze(my_super_green))

    # question 3 = if we ever uncomment the line below, does mypy raise an error ?
    # answer 3 = it does, which is a good thing
    # print(analyze(42))
