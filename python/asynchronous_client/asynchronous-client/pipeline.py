#!/usr/bin/env python3

"""
This modules implements a simple pipeline that reads and parse data from a file, and feeds it
to a consumer (through a queue) ; the consumer uses the data to request a server.
"""

import asyncio
from typing import Tuple, Dict, List, Any, TYPE_CHECKING
import multiprocessing
import multiprocessing.connection
from queue import Empty
import os

import aiohttp

if TYPE_CHECKING:
    import logging
    import pathlib
    import ctypes


def feeder(
    logger: "logging.Logger",
    queue: multiprocessing.Queue,
    exhausted_flag: "ctypes.c_bool",
    requestfile: "pathlib.Path",
    requestfile_truncate: int,
):
    """
    Feeds the queue by reading log from a requestfile.
    When read is finished, sets the exhausted_flag to True.
    If requestfile_truncate > 0, halts after reading that many log lines.
    """
    with requestfile.open("r") as f:
        for line_number, line in enumerate(f):
            if requestfile_truncate > 0 and line_number >= requestfile_truncate:
                logger.info("Just reached requestfile_truncate ({}), breaking.".format(requestfile_truncate))
                break
            request_url = line.rstrip("\n")
            queue.put(request_url)
    exhausted_flag.value = True


def consumer(
    logger: "logging.Logger",
    num_coro: int,
    queue: multiprocessing.Queue,
    exhausted_flag: "ctypes.c_bool",
    result_pipe: multiprocessing.connection.Connection,
):
    """
    Runs the consumer coroutine in an ioloop.
    """
    logger.info("Starting consumer process (pid={})".format(os.getpid()))

    results: List[Tuple[str, int, Dict[str, Any]]] = []

    coroutines = [
        consumer_coro(logger, queue, exhausted_flag, results)
        for _ in range(num_coro)
    ]

    async def gathered():
        statuses = await asyncio.gather(*coroutines)
        total_nb_success = 0
        total_nb_errors = 0
        for nb_success, nb_errors in statuses:
            total_nb_success += nb_success
            total_nb_errors += nb_errors
        result_pipe.send((results, total_nb_success, total_nb_errors))

    asyncio.run(gathered())
    logger.info("Ending (gracefully) consumer process (pid={})".format(os.getpid()))


async def consumer_coro(
    logger: "logging.Logger",
    queue: multiprocessing.Queue,
    exhausted_flag: "ctypes.c_bool",
    results: List[Tuple[str, int, Dict[str, Any]]],
) -> Tuple[int, int]:
    nb_errors = 0
    nb_success = 0

    while True:

        # process next log :
        try:
            request_url = queue.get(block=True, timeout=0.1)
        except Empty:
            # if we get here, we couldn't fetch from queue. Maybe it's time to exit ?
            if bool(exhausted_flag.value):
                break
            continue  # it wasn't time to exit : we try again to fetch another log to replay

        # if we get here, we have successfully fetched a rul to process :

        async with aiohttp.ClientSession() as session:
            async with session.get(request_url) as response:

                if response.status == 200:
                    nb_success += 1
                    logger.info(f"OK> just requested {request_url}")
                else:
                    nb_errors += 1
                    logger.warning(f"KO> got status {response.status} when requesting {request_url}")

                json_body = await response.json()

                results.append(
                    (
                        request_url,
                        response.status,
                        json_body,
                    )
                )

    logger.info("coroutine encountered {} success and {} errors".format(nb_success, nb_errors))
    return nb_success, nb_errors


def start_feeder(
    logger: "logging.Logger",
    queue: multiprocessing.Queue,
    exhausted_flag: "ctypes.c_bool",
    requestfile: "pathlib.Path",
    requestfile_truncate: int,
) -> multiprocessing.Process:
    process = multiprocessing.Process(
        target=feeder,
        args=(logger, queue, exhausted_flag, requestfile, requestfile_truncate),
    )
    process.start()
    return process


def start_consumer(
    logger: "logging.Logger",
    num_coro: int,
    queue: multiprocessing.Queue,
    exhausted_flag: "ctypes.c_bool",
) -> Tuple[multiprocessing.Process, multiprocessing.connection.Connection]:

    # using a Pipe to "return" the results :
    result_pipe_left, result_pipe_right = multiprocessing.Pipe()

    process = multiprocessing.Process(
        target=consumer,
        args=(logger, num_coro, queue, exhausted_flag, result_pipe_right),
    )

    process.start()

    return process, result_pipe_left
