#!/usr/bin/env python3

from library import PrinterHandler, DummyHandler, HandlerProtocol, check_urls
from dataclasses import dataclass
from typing import Collection, TYPE_CHECKING

if TYPE_CHECKING:
    from library import ResponseTime, ResponseCode


@dataclass
class MyCustomHandler:  # MyCustomHandler doesn't inherit from anything... yet it will be viewed as HandlerProtocol :-)
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

    In the present POC, we :
        - define an 'interfacce' (actually, a HandlerProtocol) that will "represent" all the possible handlers
        - we use typing.Protocol for this
        - type-annotate 'check_url' so that it expects a HandlerProtocol
        - no handler is EXPLICITLY related to HandlerProtocol, we could define MyCustomHandler without knowing of it...
        - ...but all handlers are still IMPLICITLY recognized as valid HandlerProtocol implementations :-)

    What happens in the POC :
        - we confirm that all handlers (including MyCustomHandler) are seen by mypy as subtypes of HandlerProtocol
        - it's IMPLICIT : MyCustomHandler implements the HandlerProtocol, even if it does not know anything about it !
        - (as 'runtime_checkable' was used, we can also use issubclass to check if MyCustomHandler is a HandlerProtocol)
        - mypy is able to properly use this 'interface' (yay !), notably all desired behaviour is ipmlemented :
            problem 1 = in library.py, mypy properly detects that the 'handler' arg implements the required methods
            problem 2 = in poc.py, mypy detects that the handlers ARE 'subtypes' of HandlerProtocol
        - moreover, if we fail to properly define a method (e.g. a typo on 'on_iteration_finish`), the typo will be
          properly "detected" by mypy
        - similarly, if we wrongly define a method (e.g. wrong argument type), mypy also detects it, very yay !
        - the only drawback : MyCustomHandler (and other handlers) must EXPLICITLY inherit from HandlerProtocol

    CONCLUSION :
        - this is my favorite way to define an interface :-)

    NOTE : Protocol/runtime_checkable are available from python >= 3.8 (for earlier versions, use 'typing_extensions')
    NOTE : as annotations are ignored at runtime, even with runtime_checkable, a type-mismatch will remain undetected...
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

    # when compared to the first POC (that used Union[...]), all the handlers are subclasses of HandlerProtocol :
    # NOTE : for this to work, the Protocol must be decorated with 'runtime_checkable'
    print("")
    print("issubclass(PrinterHandler, HandlerProtocol) = ", issubclass(PrinterHandler, HandlerProtocol))
    print("issubclass(DummyHandler, HandlerProtocol) = ", issubclass(DummyHandler, HandlerProtocol))
    print("issubclass(MyCustomHandler, HandlerProtocol) = ", issubclass(MyCustomHandler, HandlerProtocol))
    assert issubclass(PrinterHandler, HandlerProtocol)
    assert issubclass(DummyHandler, HandlerProtocol)
    assert issubclass(MyCustomHandler, HandlerProtocol)
    print("")

    # moreover, 'check_urls' now expects a HandlerProtocol (instead of a Union[...] in the first POC)
    # and all work as intended \o/
    check_urls(URLS, handler1)
    check_urls(URLS, handler2)
    check_urls(URLS, handler3)


if __name__ == "__main__":
    main()
