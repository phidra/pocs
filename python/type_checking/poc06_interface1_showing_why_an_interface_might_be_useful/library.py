from typing import Collection, Union, NewType, Tuple
import random
from dataclasses import dataclass


ResponseCode = NewType("ResponseCode", int)
ResponseTime = NewType("ResponseTime", int)


@dataclass
class PrinterHandler:
    """This handler prints the information about measured URLs."""

    total_response_time_ms: int = 0

    def on_iteration_start(self, urls: Collection[str]):
        print(f"---> ITERATION WILL START, ON {len(urls)} URLS")

    def on_url_response(self, url: str, response_time_ms: ResponseTime, response_code: "ResponseCode"):
        print(f"---> TOOK {response_time_ms} MS TO REQUEST : {url}   (response-code={response_code})")
        self.total_response_time_ms += response_time_ms

    def on_iteration_finish(self):
        print(f"---> ITERATION IS FINISHED, TOOK {self.total_response_time_ms} ms")


@dataclass
class DummyHandler:
    """This handler do nothing."""

    def on_iteration_start(self, urls: Collection[str]):
        pass

    def on_url_response(self, url: str, response_time_ms: ResponseTime, response_code: "ResponseCode"):
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


def check_urls(urls: Collection[str], handler: Union[PrinterHandler, DummyHandler]) -> None:
    handler.on_iteration_start(urls)
    for url in urls:
        # simulate request :
        response_code, response_time_ms = measure_response_time(url)

        handler.on_url_response(url, response_time_ms, response_code)
    handler.on_iteration_finish()
