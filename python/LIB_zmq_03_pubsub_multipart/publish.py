#!/usr/bin/env python3

import argparse
import zmq
import time
from typing import Tuple


def log(msg: str) -> None:
    print(f"[PUBLISHER] {msg}")


def parse_args() -> Tuple[int, str]:
    parser = argparse.ArgumentParser("This publishes multipart messages")
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

    messages = [
        "pouet1",
        "pouet2",
        "pouet3",
        "pouet4",
        "pouet5",
    ]
    log("About to publish messages, all but the last one with SNDMORE, and sleeping before sending the last one :")
    for msg in messages:
        log(msg)

    log("Sending a first (independant) message to show that connection is live :")
    zmq_socket.send("Look Mom, this message shows that connection is live !".encode("utf-8"))
    time.sleep(1)

    log("Batch sending N-1 messages with SNDMORE :")
    for msg in messages[:-1]:
        log(f"Sending {msg} with SNDMORE")
        zmq_socket.send(msg.encode("utf-8"), flags=zmq.SNDMORE)

    log("Waiting for 3 seconds")
    time.sleep(3)

    log("Sending the last message without SNDMORE :")
    msg = messages[-1]
    zmq_socket.send(msg.encode("utf-8"))

    time.sleep(0.5)


if __name__ == "__main__":
    main()
