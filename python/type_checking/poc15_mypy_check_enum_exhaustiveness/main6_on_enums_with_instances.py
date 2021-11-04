#!/usr/bin/env python3

from enum import Enum
from typing import NoReturn, Tuple
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


class Color(Enum):
    GREEN = ColorClass(0, 1, 0)
    RED = ColorClass(1, 0, 0)
    BLUE = "blue"  # (just to showcase that the instances may be of inhomogeneous types)

    # question 1 : if we ever uncomment BLACK below, does mypy detect that it's unhandled in analyze ?
    # answer : it does (mypy detects an error)
    # BLACK = ColorClass(0, 0, 0)


def assert_that_this_line_is_never_reached(value: NoReturn) -> NoReturn:
    assert False, f"Unhandled value: {value} ({type(value).__name__})"


def analyze(x: Color) -> str:
    if x is Color.RED:
        return "this is red as blood"
    elif x is Color.GREEN:
        return "this is green as grass"
    elif x is Color.BLUE:
        return "this is blue as the sea"

    # question 2 = does mypy thinks that there is an error here bc None is returned ? (when BLACK is commented above)
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

    KINDA OK> this POC is quite close to an "enum of instances" as I want it. The main problem, is I cannot use
    the instances directly (e.g. Color.GREEN), I must use the 'value' attribute (e.g. Color.GREEN.value).

    Said otherwise, Color.GREEN is not an instance of ColorClass (but Color.GREEN.value is !)

    Apart from that, mypy PERFECTLY FULFILLS the goal, on an "enum of instances" :-)
    Notably : mypy properly understands that 'analyze' handles all the possible values of 'x'.
    Moreover, if we add a new value (BLACK) to the enum, mypy detects that we have to update 'analyze'.

    As a side note, mypy is able to retrieve the proper type of 'Color.GREEN.value' :-)
    """
    )

    print(Color.GREEN.value.as_YUV())
    # print(Color.GREEN.as_YUV())  # sadly, this is not possible, a Color is not an instance of ColorClass :-(
    print(analyze(Color.GREEN))
    print(analyze(Color.RED))

    # uncomment the following lines to prove that mypy can infer the proper types :
    # reveal_type(Color.RED.value)  # Revealed type is 'main6_on_enums_with_instances.ColorClass'
    # reveal_type(Color.GREEN.value)  # Revealed type is 'main6_on_enums_with_instances.ColorClass'
    # reveal_type(Color.BLUE.value)  # Revealed type is 'builtins.str'

# question = if we ever uncomment the line below, does mypy raise an error ?
# answer = it does, which is a good thing
# analyze(42)
