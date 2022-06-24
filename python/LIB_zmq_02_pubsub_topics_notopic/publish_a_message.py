#!/usr/bin/env python3

import argparse
import zmq
import time
from typing import Tuple


def log(msg: str) -> None:
    print(f"[PUBLISHER] {msg}")


def parse_args() -> Tuple[int, str]:
    parser = argparse.ArgumentParser("This publishes a message directly, without prefixing it")
    parser.add_argument("--port", type=str, default="5678", help="ZMQ port (default: %(default)s)")
    parser.add_argument("msg", type=str, nargs=1, help="message to publish")
    args = parser.parse_args()
    return args.port, args.msg[0]


def main() -> None:
    port, msg = parse_args()
    socket_address = f"tcp://127.0.0.1:{port}"

    context = zmq.Context()
    zmq_socket = context.socket(zmq.PUB)
    zmq_socket.bind(socket_address)

    log(f"About to publish this message : '{msg}'")

    # we need to sleep to give zmq the time to fully connect.
    # See "slow joiner" in ZMQ guide : https://zguide.zeromq.org/docs/chapter1/
    time.sleep(1)

    zmq_socket.send(msg.encode("utf-8"))

    time.sleep(0.5)


if __name__ == "__main__":
    main()
