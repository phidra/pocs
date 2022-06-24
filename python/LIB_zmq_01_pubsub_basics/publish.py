#!/usr/bin/env python3

import argparse
import zmq
import time
from typing import Tuple, List


def log(msg: str) -> None:
    print(f"[PUBLISHER] {msg}")


def parse_args() -> Tuple[int, List[str]]:
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", type=str, default="5678", help="ZMQ port (default: %(default)s)")
    parser.add_argument("msgs", metavar="msg", type=str, nargs="+", help="messages to publish")
    args = parser.parse_args()
    return args.port, args.msgs


def main() -> None:
    port, msgs = parse_args()
    socket_address = f"tcp://127.0.0.1:{port}"

    context = zmq.Context()
    zmq_socket = context.socket(zmq.PUB)
    zmq_socket.bind(socket_address)

    log(f"About to publish the following messages : {'+'.join(msgs)}")

    # we need to sleep to give zmq the time to fully connect.
    # See "slow joiner" in ZMQ guide : https://zguide.zeromq.org/docs/chapter1/
    time.sleep(1)

    for msg in msgs:
        msg_bytes = msg.encode("utf-8")
        zmq_socket.send(msg_bytes)
        time.sleep(0.5)

    log(f"Done, {len(msgs)} were published")


if __name__ == "__main__":
    main()
