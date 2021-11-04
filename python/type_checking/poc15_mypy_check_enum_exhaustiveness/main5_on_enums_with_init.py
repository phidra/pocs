#!/usr/bin/env python3

from enum import Enum
from typing import NoReturn, Tuple


class Color(Enum):
    GREEN = (0, 1, 0)
    RED = (1, 0, 0)
    BLUE = (0, 0, 1)

    def __init__(self, R: float, G: float, B: float):
        self.R = R
        self.G = G
        self.B = B

    def as_RGB(self) -> Tuple[float, float, float]:
        return self.R, self.G, self.B

    def as_YUV(self) -> Tuple[float, float, float]:
        Y = 0.257 * self.R + 0.504 * self.G + 0.098 * self.B + 16
        U = -0.148 * self.R - 0.291 * self.G + 0.439 * self.B + 128
        V = 0.439 * self.R - 0.368 * self.G - 0.071 * self.B + 128
        return Y, U, V

    def __repr__(self):
        return f"Color(R={self.R}, G={self.G}, B={self.B})"


def assert_that_this_line_is_never_reached(value: NoReturn) -> NoReturn:
    assert False, f"Unhandled value: {value} ({type(value).__name__})"


def analyze(x: Color) -> str:
    if x is Color.RED:
        return "this is red as blood"
    elif x is Color.GREEN:
        return "this is green as grass"
    elif x is Color.BLUE:
        return "this is blue as the sea"

    # question = does mypy thinks that there is an error here bc None is returned ?
    # answer = it does (which is a BAD thing !) because it can't understand that we never reach this line
    assert_that_this_line_is_never_reached(x)


if __name__ == "__main__":
    print(
        """
    All these POCs are part of a same group of POCs, whose goal is to define an "enum of instances" where mypy
    will detect if we fail to handle all the possible instances.
    Said otherwise : we want mypy to enforce that we exhaustively handled all the possible cases in an "enum".

    (the runtime is quite useless, the interest is to see how mypy analyzes the present file, and how it
    reacts when we uncomment some of its content)

    KO> this POC shows that the trick used in previous POC does not work on enum that have __init__.
    The reason is unclear, this might be a bug in mypy, that I reported :
        https://github.com/python/mypy/issues/11461

    Note even with an __init__, the enum is still unextendable :
        a_brand_new_color = Color(0.5, 0.5, 0.5)
        # the previous line raises either of those exceptions :
        #   TypeError: __call__() takes from 2 to 3 positional arguments but 4 were given
        #   TypeError: Cannot extend enumerations
    """
    )

    print("blue YUV = ", Color.BLUE.as_YUV())
    print(analyze(Color.RED))
    print(analyze(Color.GREEN))

# question = if we ever uncomment the line below, does mypy raise an error ?
# answer = it does, which is a good thing
# analyze(42)
