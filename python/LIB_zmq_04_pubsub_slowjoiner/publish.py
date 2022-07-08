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
    args = parser.parse_args()
    return args.port


def main() -> None:
    port = parse_args()
    socket_address = f"tcp://127.0.0.1:{port}"

    context = zmq.Context()
    zmq_socket = context.socket(zmq.PUB)
    zmq_socket.bind(socket_address)

    log(f"About to publish the following messages without waiting for live connection :")

    for i in range(5):
        # we need to sleep to give zmq the time to fully connect.
        # See "slow joiner" in ZMQ guide : https://zguide.zeromq.org/docs/chapter1/
        # BUT here, we do not sleep before publishing (intentionnaly, it's for the POC) :
        msg = f"this message will probably never be received #{i}".encode("utf-8")
        log(f"Sending msg : {msg}")
        zmq_socket.send(msg)

    log(f"Now sleeping to wait for live connection :")
    # now sleep before sending
    time.sleep(1)

    log(f"Now that connection should be live, sending a message :")
    zmq_socket.send(b"this message should be properly received \o/")


if __name__ == "__main__":
    main()
