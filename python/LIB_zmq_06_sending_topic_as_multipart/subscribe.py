#!/usr/bin/env python3

import argparse
import json

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
    topic = b"interesting_topic"
    zmq_socket.setsockopt(zmq.SUBSCRIBE, topic)  # only subscribe to the topic
    log("We will only get messages with this topic : {repr(topic)}")
    zmq_socket.connect(socket_address)

    log("About to recv in an infinite loop :")
    while True:
        # each loop expects a 2-part messages : first, we receive the topic :
        received_topic = zmq_socket.recv()

        # now, we receive the actual data :
        received_bytes = zmq_socket.recv()
        received_str = received_bytes.decode("utf-8")
        log(f"received {received_str}")
        received_dict = json.loads(received_str)
        log(f"received age  was {received_dict['age']}")
        log(f"received name was {received_dict['name']}")


if __name__ == "__main__":
    main()
