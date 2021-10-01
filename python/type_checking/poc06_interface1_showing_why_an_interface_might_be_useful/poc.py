#!/usr/bin/env python3

from library import PrinterHandler, DummyHandler, check_urls
from dataclasses import dataclass
from typing import Collection, TYPE_CHECKING

if TYPE_CHECKING:
    from library import ResponseTime, ResponseCode


@dataclass
class MyCustomHandler:
    """This is a custom handler (unknown in the library, and notably absent from the Union)."""

    def on_iteration_start(self, urls: Collection[str]):
        print("CUSTOM> on_iteration_start")

    def on_url_response(self, url: str, response_time_ms: "ResponseTime", response_code: "ResponseCode"):
        print("CUSTOM> on_url_response")

    def on_iteration_finish(self):
        print("CUSTOM> on_iteration_finish")


def main() -> None:  # main is annotated to allow mypy to run

    poc_description = """
    The purpose of the POC is to show why one would need to define an 'interface' in python.

    More details :
        - a library provides a function 'check_urls', that processes a bunch of URLs
        - this function expects a handler as parameter, which, by implementing some functions, acts as a hook
        - for instance, 'handler.on_iteration_start' is called before iterating on URLs
        - the library also provides two useful handlers : PrinterHandler (to log) and DummyHanlder (to do nothing)

    In this POC, the relevant thing is the type-checking :
        - the function 'check_urls` expects the handler
        - in this POC's implementation, it is type-checked as 'Union[PrinterHandler, DummyHandler]'...
        - ...BUT this is far from ideal ! Notably, the library-user cannot define his own custom Handler.
        - this is illustrated with handler3, which is valid at runtime, but not at type-checking-time
        - what we would like is to describe the constraints on the expected Handler
        - (e.g. "handler.on_iteration_start" must be a valid call that accepts a Collection[str])

    DESIRED BEHAVIOUR : we want mypy to acknowledge MyCustomHandler as a valid type for check_urls second parameter.

    CONCLUSION : we would like to describe the expected handler in an INTERFACE, QED

    ADDITIONAL NOTE : the question here is for type-checking (there is no runtime-problem : at runtime, either the
                      handler implement valid methods, or program will raise NameError or will misbehave.
                      So the question is specifically "how to annotate the type of a Handler interface ?".
                      This POC doesn't provide an answer though, it just illustrates the problem ;-)
    """

    print(poc_description)

    URLS = [
        "https://www.google.fr",
        "https://fr.wikipedia.org",
        "https://www.python.org",
    ]
    handler1 = PrinterHandler()
    handler2 = DummyHandler()
    handler3 = MyCustomHandler()
    check_urls(URLS, handler1)
    check_urls(URLS, handler2)

    # the following call is perfectly valid, but mypy (rightfully) complains, because MyCustomHandler
    # is not declared in the Union[...] that 'check_urls' expects...
    check_urls(URLS, handler3)


if __name__ == "__main__":
    main()
