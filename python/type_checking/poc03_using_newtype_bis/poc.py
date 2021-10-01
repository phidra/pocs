#!/usr/bin/env python3


from typing import Collection

import random
from typing import NewType, Tuple


# Two DIFFERENT int-linke types :
ResponseCode = NewType("ResponseCode", int)
ResponseTime = NewType("ResponseTime", int)


def measure_response_time(url: str) -> Tuple[ResponseCode, ResponseTime]:
    """
    This functions simulate the request of an URL and returns its (simulated) response-time.
    The important thing is that the function return 2 incompatible int-linke types : ResponseCode and ResponseTime
    """
    time = random.randint(100, 999)
    code = random.choice((200, 200, 200, 200, 200, 200, 200, 404, 400, 403, 500))
    return ResponseCode(code), ResponseTime(time)


def show_time_OK__BUG_IS_DETECTED_BY_MYPY(url: str, response_time_ms: ResponseTime):
    """
    With this version of the function, the bug IS detected by mypy.
    Indeed, this version expects a ResponseTime, and the passed ResponseCode is not convertible to ResponseTime
    Thus, mypy detects that we pass an incompatible type.
    """
    print(f"TOOK {response_time_ms} MS TO REQUEST : {url}")


def show_time_KO__BUG_IS_UNDETECTED_BY_MYPY(url: str, response_time_ms: int):
    """
    With this version of the function, the bug IS NOT detected by mypy.
    Indeed, this version expects an int, and both ResponseCode and ResponseTime are implicitly convertible to int...
    See https://docs.python.org/3/library/typing.html#newtype:
        This lets you pass in a UserId wherever an int might be expected
    """
    print(f"TOOK {response_time_ms} MS TO REQUEST : {url}")


def check_urls(urls: Collection[str]) -> None:
    for url in urls:
        # the line below has a BUG, as response_time and response_code are switched !
        response_time_ms, response_code = measure_response_time(url)

        # below, the variable name is misleading, as the value passed is not the response_time but the response_code.
        # the question is : will mypy detect this bug ?
        # the answer is... it depends !
        show_time_KO__BUG_IS_UNDETECTED_BY_MYPY(url, response_time_ms)  # <-- mypy will not detect the bug here  :-(
        show_time_OK__BUG_IS_DETECTED_BY_MYPY(url, response_time_ms)  # <--- mypy should detect the bug here  \o/


def main() -> None:  # main is annotated to allow mypy to run

    poc_description = """
    The purpose of the POC is to show that defining NewType doesn't automagically prevent bugs.
    Instead, one must ACTUALLY USE them properly for mypy to catch bugs...

    More details :

        - a function 'measure_response_time' returns a tuple of 2 NewType (int-like) : ResponseCode and ResponseTime
        - when this function is called, there is a (intentional) bug at call site : code and time are switched !
        - later another function 'show_time' tries to display the time returned by the function
        - but because of the bug, we actually pass a ResponseCode to this function (instead of a ResponseTime)

    The question is : will mypy detect this bug ?
    The answer is, it depends on 'show_time' signature, the POC implements 2 versions to illustrate the difference :
        - if 'show_time' expects an int, mypy DOES NOT detect the bug (as ResponseCode AND ResponseTime derive from int)
        - if it expects a ResponseTime, mypy DO detect the bug (we expect a Time, but got an incompatible Code instead)

    MORAL OF THE STORY : NewType is not magical, kids : you have to actually use them properly to benefit from them
    """

    print(poc_description)

    URLS = [
        "https://www.google.fr",
        "https://fr.wikipedia.org",
        "https://www.python.org",
    ]
    check_urls(URLS)  # the actual result of the function is useless : the purpose of the POC is in mypy warnings


if __name__ == "__main__":
    main()
