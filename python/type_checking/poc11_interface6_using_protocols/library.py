from typing import Collection, NewType, Tuple, Protocol, runtime_checkable
import random
from dataclasses import dataclass


ResponseCode = NewType("ResponseCode", int)
ResponseTime = NewType("ResponseTime", int)


@runtime_checkable
class HandlerProtocol(Protocol):  # here, HandlerProtocol is a Protocol
    def on_iteration_start(self, urls: Collection[str]):
        ...  # note the particular syntax...

    def on_url_response(self, url: str, response_time_ms: ResponseTime, response_code: ResponseCode):
        ...

    def on_iteration_finish(self):
        ...


@dataclass
class PrinterHandler:  # note that PrinterHandler is not explicitly related to HandlerProtocol
    """This handler prints the information about measured URLs."""

    total_response_time_ms: int = 0

    def on_iteration_start(self, urls: Collection[str]):
        print(f"---> ITERATION WILL START, ON {len(urls)} URLS")

    def on_url_response(self, url: str, response_time_ms: ResponseTime, response_code: ResponseCode):
        print(f"---> TOOK {response_time_ms} MS TO REQUEST : {url}   (response-code={response_code})")
        self.total_response_time_ms += response_time_ms

    def on_iteration_finish(self):
        print(f"---> ITERATION IS FINISHED, TOOK {self.total_response_time_ms} ms")


@dataclass
class DummyHandler:  # note that DummyHandler is not explicitly related to HandlerProtocol
    """This handler does nothing."""

    def on_iteration_start(self, urls: Collection[str]):
        pass

    def on_url_response(self, url: str, response_time_ms: ResponseTime, response_code: ResponseCode):
        pass

    def on_iteration_finish(self):
        pass


def measure_response_time(url: str) -> Tuple[ResponseCode, ResponseTime]:
    """
    This functions simulate the request of an URL and returns its (simulated) response-time.
    The important thing is that the function return 2 incompatible int-linke types : ResponseCode and ResponseTime
    """
    time = random.randint(100, 999)
    code = random.choice((200, 200, 200, 200, 200, 200, 200, 404, 400, 403, 500))
    return ResponseCode(code), ResponseTime(time)


# NOTE 2 : when compare to the previous POC, the typing of 'handler' has changed.
# it now uses our 'HandlerProtocol'.
# The goal is for mypy to be able to recognize our MyCustomHandler as a HandlerProtocol.
# And it works \o/
def check_urls(urls: Collection[str], handler: HandlerProtocol) -> None:
    handler.on_iteration_start(urls)
    for url in urls:
        # simulate request :
        response_code, response_time_ms = measure_response_time(url)

        handler.on_url_response(url, response_time_ms, response_code)
    handler.on_iteration_finish()
