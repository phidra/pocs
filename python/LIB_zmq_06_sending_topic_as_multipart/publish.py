#!/usr/bin/env python3

import argparse
import zmq
import time
import json
from pprint import pprint
from typing import Tuple


def log(msg: str) -> None:
    print(f"[PUBLISHER] {msg}")


def parse_args() -> Tuple[int, str]:
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", type=str, default="5678", help="ZMQ port (default: %(default)s)")
    args = parser.parse_args()
    return args.port


def main() -> None:
    port = parse_args()
    socket_address = f"tcp://127.0.0.1:{port}"

    context = zmq.Context()
    zmq_socket = context.socket(zmq.PUB)
    zmq_socket.bind(socket_address)

    # subscriber was run first, so we need to sleep to give publisher zmq the time to fully connect.
    # See "slow joiner" in ZMQ guide : https://zguide.zeromq.org/docs/chapter1/
    time.sleep(1)

    data_to_send = {
        "name": "luke",
        "age": 42,
    }
    log("About to send this data without topic, it will NOT be received (missing the topic) :")
    pprint(data_to_send)
    zmq_socket.send(json.dumps(data_to_send).encode("utf-8"))

    log("Now sending the same data, but as multipart, with topic as first message :")
    pprint(data_to_send)
    zmq_socket.send(b"interesting_topic", flags=zmq.SNDMORE)
    zmq_socket.send(json.dumps(data_to_send).encode("utf-8"))


if __name__ == "__main__":
    main()
