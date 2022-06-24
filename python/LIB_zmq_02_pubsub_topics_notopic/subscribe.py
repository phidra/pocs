#!/usr/bin/env python3

import argparse
from typing import List
import concurrent.futures
import time

import zmq


def log(msg: str) -> None:
    print(f"[SUBSCRIBER] {msg}")


def parse_args() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", type=str, default="5678", help="ZMQ port (default: %(default)s)")
    args = parser.parse_args()
    return args.port


def _start_receiving(zmq_socket: zmq.Socket, name: str) -> None:
    log(f"RECEIVER {name}> About to recv in an infinite loop")
    while True:
        received_bytes = zmq_socket.recv()
        received_str = received_bytes.decode("utf-8")
        log(f"{name}> {received_str}")


def spawn_subscriber_no_topic(socket_address: str, name: str) -> zmq.Socket:
    zmq_socket = zmq.Context().instance().socket(zmq.SUB)
    zmq_socket.connect(socket_address)
    _start_receiving(zmq_socket, name)


def spawn_subscriber_with_topic(socket_address: str, name: str, topic: bytes) -> zmq.Socket:
    zmq_socket = zmq.Context().instance().socket(zmq.SUB)
    zmq_socket.setsockopt(zmq.SUBSCRIBE, topic)  # using this topic
    zmq_socket.connect(socket_address)
    _start_receiving(zmq_socket, name)


def spawn_subscriber_with_multiple_topics(socket_address: str, name: str, topics: List[bytes]) -> zmq.Socket:
    zmq_socket = zmq.Context().instance().socket(zmq.SUB)
    for topic in topics:
        zmq_socket.setsockopt(zmq.SUBSCRIBE, topic)
    zmq_socket.connect(socket_address)
    _start_receiving(zmq_socket, name)


def main() -> None:
    port = parse_args()
    socket_address = f"tcp://127.0.0.1:{port}"
    log(f"Starting several subscribers that listen to : {socket_address}")

    with concurrent.futures.ThreadPoolExecutor(max_workers=5) as executor:

        # subscriber without topic -> won't receive any messages :
        executor.submit(spawn_subscriber_no_topic, socket_address, "no_topic")

        # subscriber with specific topic -> will only receive matching messages :
        executor.submit(spawn_subscriber_with_topic, socket_address, "topic_A", b"A")
        executor.submit(spawn_subscriber_with_topic, socket_address, "topic_B", b"B")

        # subscriber with empty topic (which is different from no topic !) -> will receive every messages :
        executor.submit(spawn_subscriber_with_topic, socket_address, "empty_topic", b"")

        # subscriber can have multiple topics -> will receive matching messages with all topics:
        executor.submit(spawn_subscriber_with_multiple_topics, socket_address, "both_A_and_B", [b"A", b"B"])

        time.sleep(0.5)
        log("Ready to receive messages !")


if __name__ == "__main__":
    main()
