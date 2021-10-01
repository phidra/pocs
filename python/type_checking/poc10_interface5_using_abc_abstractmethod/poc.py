#!/usr/bin/env python3

from library import PrinterHandler, DummyHandler, HandlerInterface, check_urls
from dataclasses import dataclass
from typing import Collection, TYPE_CHECKING

if TYPE_CHECKING:
    from library import ResponseTime, ResponseCode


@dataclass
class MyCustomHandler(HandlerInterface):
    """This is a custom handler (unknown in the library, and notably absent from the Union)."""

    def on_iteration_start(self, urls: Collection[str]):
        print("CUSTOM> on_iteration_start")

    # if uncommented, the following argument mismatch (int vs. str) will properly be "detected" by mypy, yay !
    # def on_url_response(self, url: int, response_time_ms: "ResponseTime", response_code: "ResponseCode"):
    def on_url_response(self, url: str, response_time_ms: "ResponseTime", response_code: "ResponseCode"):
        print("CUSTOM> on_url_response")

    # if uncommented, the following typo will properly be "detected" by mypy, yay !
    # def on_iteration_finishh(self):
    def on_iteration_finish(self):
        print("CUSTOM> on_iteration_finish")


def main() -> None:  # main is annotated to allow mypy to run

    poc_description = """
    This POC is the sequel of the first POC on python interfaces (which describes the problem and the desired behaviour)

    In the present POC, we try to :
        - define an interface HandlerInterface that will "represent" all the possible handlers
        - we use the decorator @abc.abstract for this, BUT we apparently don't need the HandlerInterface to be a ABC ?!
        - (this is different from : cf. https://realpython.com/python-interface/#using-abstract-method-declaration)
        - type-annotate 'check_url' so that it expects a HandlerInterface
        - all handlers (including MyCustomHandler) inherit from HandlerInterface

    What happens in the POC :
        - we confirm that all handlers (including MyCustomHandler) are properly seen as subtypes of HandlerInterface
        - mypy is able to properly use this interface (yay !), notably the 2 problems of previous poc are solved :
            problem 1 = in library.py, mypy now properly detect that the handler arg implements the required methods
            problem 2 = in poc.py, mypy now detect that the handlers ARE subtypes of HandlerInterface
        - moreover, if we fail to properly define a method (e.g. a typo on 'on_iteration_finish`), the typo will be
          properly "detected" by mypy
        - similarly, if we wrongly define a method (e.g. wrong argument type), mypy also detects it, very yay !
        - NOTE : there is no need to use ABCMeta, only abc.abstractmethod ...

            this is an apparent contradiction with the doc
            https://docs.python.org/fr/3/library/abc.html#abc.abstractmethod
            "Using this decorator requires that the class’s metaclass is ABCMeta or is derived from it."
            but what seems to require the use of the metaclass, is the RUNTIME behaviour (i.e. preventing the
            instanciation of a child-class of an ABC that didn't properly implement the required methods)
        - the only drawback : MyCustomHandler (and other handlers) must EXPLICITLY inherit from HandlerInterface

    CONCLUSION :
        - this is a pretty solid way of defining an interface !
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
