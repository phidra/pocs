#!/usr/bin/env python3

from enum import Enum, auto
from typing import NoReturn


class Color(Enum):
    GREEN = auto()
    RED = auto()
    BLUE = auto()


def assert_that_this_line_is_never_reached(value: NoReturn) -> NoReturn:
    """
    When this function is called with an argument, mypy will ALWAYS complain.
    The only way for mypy not to complain, is if it can know for sure that the call is never reached
    We use this "trick" below to detect when a enum case is left unhandled.
    """
    assert False, f"Unhandled value: {value} ({type(value).__name__})"


def analyze_ok(x: Color) -> None:  # ok because mypy PROPERLY detect that we forgot to handle a case, yay !
    if x is Color.RED:
        print("this is red as blood")
    elif x is Color.GREEN:
        print("this is green as grass")
    # here, we FAIL to handle the BLUE case !

    # question = does mypy detect that there is an error here because handling BLUE case is missing ?
    # answer = yep, it does (which is a GOOD thing !) because the following assert will make mypy complain, unless
    #          mypy is sure that we never reach this line.
    assert_that_this_line_is_never_reached(x)  # calling this here is the trick !


def analyze_ko(x: Color) -> None:  # ko because mypy FAILS to detect that we forgot to handle a case...
    if x is Color.RED:
        print("this is red as blood")
    elif x is Color.GREEN:
        print("this is green as grass")
    # here, we FAIL to handle the BLUE case !

    # question = does mypy detect that there is an error here because handling BLUE case is missing ?
    # answer = sadly, it does not (which is a BAD thing !) because even if it knows that this line is reachable,
    #          the return type is compatible with the function signature...


if __name__ == "__main__":
    print(
        """
    All these POCs are part of a same group of POCs, whose goal is to define an "enum of instances" where mypy
    will detect if we fail to handle all the possible instances.
    Said otherwise : we want mypy to enforce that we exhaustively handled all the possible cases in an "enum".

    (the runtime is quite useless, the interest is to see how mypy analyzes the present file, and how it
    reacts when we uncomment some of its content)

    OK> The current POC shows that the previous POCs actually worked by luck : we show an example (analyze_ko) where
    we fail to handle an enum case, and mypy still doesn't detect it...
    The current POC also provides a trick to always detect the unhandled cases, which is inspired by :
        https://github.com/python/mypy/issues/6366
        https://hakibenita.com/python-mypy-exhaustive-checking

    Why did it work by accident ?
        - actually mypy cannot detect that we forgot to handle an enum case
        - what mypy actually detects, is if the returned type is incompatible
        - in the previous POCs, forgotting to handle a case lead to return None, which was incompatible
        - but in the current POC, None is a valid return-type for 'analyze_ko', thus the error is undetected...

    How does the 'assert_never' trick work ?
        - we define a function that, if called, will ALWAYS be considered invalid by mypy
        - (in order to do that, the function accepts a 'NoReturn' as input parameter)
        - so when this function is called with an argument, mypy will ALWAYS complain.
        - the only way for mypy not to complain, is if it can know for sure that the call is never made
        - to know that, we rely on type-narrowing :
            https://mypy.readthedocs.io/en/latest/type_narrowing.html
        - if we properly handle all the cases, type-narrowing ensures that the assert is unreachable
        - but if we fail to handle all the cases, the assert becomes reachable, and mypy complains about it :-)
    """
    )

analyze_ok(Color.RED)
analyze_ok(Color.GREEN)

analyze_ko(Color.RED)
analyze_ko(Color.GREEN)

# question = if we ever uncomment the line below, does mypy raise an error ?
# answer = it does, which is a good thing
# analyze_ko(42)
# analyze_ok(42)
