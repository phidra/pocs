#!/usr/bin/env python3

from library import PrinterHandler, DummyHandler, HandlerInterface, check_urls
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

    # def on_iteration_finishh(self):  # this typo will properly be "detected" by issubclass returning False
    def on_iteration_finish(self):
        print("CUSTOM> on_iteration_finish")


def main() -> None:  # main is annotated to allow mypy to run

    poc_description = """
    This POC is the sequel of the first POC on python interfaces (which describes the problem and the desired behaviour)

    This POC is similar (slightly different implementation, same outcome) that the previous POC that uses a Metaclass.

    In the present POC, we try to :
        - define an interface HandlerInterface that will "represent" all the possible handlers
        - (we use abc.ABCMeta for this, cf. https://realpython.com/python-interface/#using-metaclasses)
        - type-annotate 'check_url' so that it expects a HandlerInterface
        - see if MyCustomHandler (which is properly seen as a subclass of HandlerInterface) is accepted by check_urls

    What happens in the POC :

        - the outcome of the POC is the same than the POC which uses a (regular) metaclass to define HandlerInterface
        - (see this POC description)
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

    # when compared to the previous POC (that used Union[...]), all the handlers are subclasses of HandlerInterface :
    print("")
    print("issubclass(PrinterHandler, HandlerInterface) = ", issubclass(PrinterHandler, HandlerInterface))
    print("issubclass(DummyHandler, HandlerInterface) = ", issubclass(DummyHandler, HandlerInterface))
    print("issubclass(MyCustomHandler, HandlerInterface) = ", issubclass(MyCustomHandler, HandlerInterface))
    assert issubclass(PrinterHandler, HandlerInterface)
    assert issubclass(DummyHandler, HandlerInterface)
    assert issubclass(MyCustomHandler, HandlerInterface)
    print("")

    # moreover, 'check_urls' now expects a HandlerInterface (instead of a Union[...] in the previous POC)
    # sadly, mypy doesn't recognized none of the handlers as HandlerInterface :
    # (and in 'library', mypy doesn't seem to understand that HandlerInterface implements the used methods...)
    check_urls(URLS, handler1)
    check_urls(URLS, handler2)
    check_urls(URLS, handler3)


if __name__ == "__main__":
    main()
