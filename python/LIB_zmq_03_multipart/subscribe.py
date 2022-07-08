#!/usr/bin/env python3

import argparse
import datetime

import zmq


def log(msg: str) -> None:
    print(f"[SUBSCRIBER] {msg}")


def parse_args() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", type=str, default="5678", help="ZMQ port (default: %(default)s)")
    args = parser.parse_args()
    return args.port


def main() -> None:
    port = parse_args()
    socket_address = f"tcp://127.0.0.1:{port}"
    log(f"Subscribing to : {socket_address}")

    context = zmq.Context().instance()
    zmq_socket = context.socket(zmq.SUB)
    zmq_socket.setsockopt(zmq.SUBSCRIBE, b"")
    zmq_socket.connect(socket_address)

    log("About to recv in an infinite loop and displaying time elapsed since last recv")
    last_recv_datetime = None
    while True:
        received_bytes = zmq_socket.recv()
        received_str = received_bytes.decode("utf-8")

        # time elapsed since last recv :
        if last_recv_datetime is None:
            elapsed = 0
        else:
            elapsed = (datetime.datetime.now() - last_recv_datetime).total_seconds()

        last_recv_datetime = datetime.datetime.now()
        log(f"RECEIVED> (after {elapsed:.1f} seconds) {received_str}")


if __name__ == "__main__":
    main()
