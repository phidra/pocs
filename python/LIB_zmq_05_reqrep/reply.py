#!/usr/bin/env python3

import argparse
import json

import zmq


def log(msg: str) -> None:
    print(f"[SERVER] {msg}")


def parse_args() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", type=str, default="5678", help="ZMQ port (default: %(default)s)")
    args = parser.parse_args()
    return args.port


def main() -> None:
    port = parse_args()
    socket_address = f"tcp://127.0.0.1:{port}"
    log(f"Replying to requests on : {socket_address}")

    context = zmq.Context().instance()
    zmq_socket = context.socket(zmq.REP)
    zmq_socket.bind(socket_address)

    log("About to recv+send in an infinite loop")
    while True:
        received_bytes = zmq_socket.recv()
        received_str = received_bytes.decode("utf-8")
        log(f"received {received_str}")
        received_dict = json.loads(received_str)
        response = {
            "age": received_dict.get("age", 42) * 2,  # double the age
            "name": received_dict.get("name", "undefined").upper(),  # upper the name
        }
        to_send = json.dumps(response)
        log(f"about to reply with {to_send}")
        zmq_socket.send(to_send.encode("utf-8"))


if __name__ == "__main__":
    main()
