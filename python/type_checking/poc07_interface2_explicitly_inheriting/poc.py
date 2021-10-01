#!/usr/bin/env python3

from library import PrinterHandler, DummyHandler, HandlerInterface, check_urls
from dataclasses import dataclass
from typing import Collection, TYPE_CHECKING

if TYPE_CHECKING:
    from library import ResponseTime, ResponseCode


@dataclass
class MyCustomHandler(HandlerInterface):  # implements HandlerInterface... or does it ?
    """This is a custom handler (unknown in the library, and notably absent from the Union)."""

    def on_iteration_start(self, urls: Collection[str]):
        print("CUSTOM> on_iteration_start")

    def on_url_response(self, url: str, response_time_ms: "ResponseTime", response_code: "ResponseCode"):
        print("CUSTOM> on_url_response")

    def on_iteration_finishh(self):  # there is a typo here, the method doesn't override `on_iteration_finish' !
        print("CUSTOM> on_iteration_finish")


def main() -> None:  # main is annotated to allow mypy to run

    poc_description = """
    This POC is the sequel of the first POC on python interfaces (which describes the problem and the desired behaviour)

    In the present POC, we try to :
        - define an interface HandlerInterface that will "represent" all the possible handlers
        - (we use a simple class with "explicitly not implemented" methods, that raises NotImplementedError)
        - type-annotate 'check_url' so that it expects a HandlerInterface
        - all the handlers (the ones in 'library.py', and also MyCustomHandler) EXPLICITLY inherit from HandlerInterface

    What happens in the POC :
        - benefit : the code is mypy compliant : mypy recognize the 3 handlers as valid input types for 'check_url'
        - benefit : it IS possible to define on client-side a custom handler, usable with 'check_url'
        - (maybe minor) drawback : the client-side custom handler MUST inherit from HandlerInterface
            (this is not always possible/wanted, especially if a class must inherit from several library's interfaces)
            (we want the code to be coupled only to 'check_url', but is is now also coupled to the library's interface)
        - MAJOR drawback : nothing (not even mypy) forces the client-code to properly define the required methods :
            in the POC, there is a bug (typo) with the client-side handler : it doesn't define 'on_iteration_finish'
            consequently, the code FAILS at runtime... and mypy didn't detect this bug (it complain at all !)

    CONCLUSION :
        using a parent-class to define interfaces is not a silver-bullet

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

    # when compared to the first POC (that used Union[...]), all the handlers are subclasses of HandlerInterface :
    print("")
    print("issubclass(PrinterHandler, HandlerInterface) = ", issubclass(PrinterHandler, HandlerInterface))
    print("issubclass(DummyHandler, HandlerInterface) = ", issubclass(DummyHandler, HandlerInterface))
    print("issubclass(MyCustomHandler, HandlerInterface) = ", issubclass(MyCustomHandler, HandlerInterface))
    assert issubclass(PrinterHandler, HandlerInterface)
    assert issubclass(DummyHandler, HandlerInterface)
    assert issubclass(MyCustomHandler, HandlerInterface)
    print("")

    # moreover, 'check_urls' now expects a HandlerInterface (instead of a Union[...] in the first POC)
    # this makes mypy happy... but fails to detect a bug in our custom client-side handler :-(
    check_urls(URLS, handler1)
    check_urls(URLS, handler2)
    check_urls(URLS, handler3)


if __name__ == "__main__":
    main()
