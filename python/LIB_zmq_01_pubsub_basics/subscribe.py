#!/usr/bin/env python3

import argparse

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

    log("About to recv in an infinite loop")
    while True:
        received_bytes = zmq_socket.recv()
        received_str = received_bytes.decode("utf-8")
        log(f"RECEIVED> {received_str}")


if __name__ == "__main__":
    main()
