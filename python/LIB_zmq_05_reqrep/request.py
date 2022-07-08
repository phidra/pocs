#!/usr/bin/env python3

import argparse
import time
import json
from pprint import pprint
from typing import Tuple, List

import zmq


def log(msg: str) -> None:
    print(f"[CLIENT] {msg}")


def parse_args() -> Tuple[int, List[str]]:
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", type=str, default="5678", help="ZMQ port (default: %(default)s)")
    parser.add_argument("--name", type=str, default="luke", help="name to send (default: %(default)s)")
    parser.add_argument("--age", type=int, default=30, help="age to send (default: %(default)s)")
    args = parser.parse_args()
    return args.port, args.name, args.age


def main() -> None:
    port, name, age = parse_args()
    socket_address = f"tcp://127.0.0.1:{port}"

    context = zmq.Context()
    zmq_socket = context.socket(zmq.REQ)
    zmq_socket.connect(socket_address)

    # no slow joiner problem here, yay \o/

    data_to_send = {
        "name": name,
        "age": age,
    }
    log("About to request with this data :")
    pprint(data_to_send)
    zmq_socket.send(json.dumps(data_to_send).encode("utf-8"))

    # getting reply :
    response = zmq_socket.recv()
    log("got this data in response :")
    pprint(json.loads(response.decode("utf-8")))


if __name__ == "__main__":
    main()
