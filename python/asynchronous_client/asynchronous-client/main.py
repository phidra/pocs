#!/usr/bin/env python3

import argparse
import sys
import logging
import pathlib
import multiprocessing
from ctypes import c_bool
import textwrap
from typing import cast

from pipeline import start_feeder, start_consumer
from results import aggregate_results, dump_results

DEFAULT_TARGET_FILENAME = "NOGIT_results.json"


def init_logging() -> logging.Logger:
    logformat = "%(asctime)s|%(levelname)8s|%(name)s|   %(message)s"
    logging.basicConfig(level=logging.INFO, format=logformat)
    return logging.getLogger("request-replayer")


def parse_arguments() -> argparse.Namespace:
    description = textwrap.dedent(
        """
        consume the requests from an input requestfile, replays the requests, and store json responses.
    """
    )
    parser = argparse.ArgumentParser(
        description=description,
        formatter_class=argparse.RawTextHelpFormatter,
    )

    parser.add_argument(
        "requestfile",
        type=str,
        help="path to input requestfile containing requests (e.g. 'path/to/requests.txt')",
    )

    parser.add_argument(
        "--requestfile-truncate",
        type=int,
        default=0,
        help="If >0, only process the first N requests in requestfile (default=%(default)s)",
    )

    parser.add_argument(
        "--num-client-processes",
        type=int,
        default=1,
        help="number of client processes to spawn (default=%(default)i)",
    )

    parser.add_argument(
        "--num-client-coro",
        type=int,
        default=20,
        help="number of client coroutines per process to spawn (default=%(default)i)",
    )

    parser.add_argument(
        "--output",
        type=str,
        default="",
        help="output file (default='{}', in the current folder".format(DEFAULT_TARGET_FILENAME),
    )

    args = parser.parse_args()

    # requestfile must be an existing file :
    try:
        args.requestfile = pathlib.Path(args.requestfile).expanduser().resolve()
    except FileNotFoundError:
        raise ValueError("unexisting requestfile : {}".format(args.requestfile))

    if args.output == "":
        args.output = pathlib.Path(sys.argv[0]).expanduser().resolve().parent / DEFAULT_TARGET_FILENAME
    args.output = pathlib.Path(args.output)

    return args


if __name__ == "__main__":
    logger = init_logging()
    args = parse_arguments()

    logger.info("Replaying requestfile         : {}".format(args.requestfile))
    logger.info("Dumping to outputfile         : {}".format(args.output))
    logger.info("Number of client processes    : {}".format(args.num_client_processes))
    logger.info("Number of coroutines/process  : {}".format(args.num_client_coro))
    if args.requestfile_truncate > 0:
        logger.info("Truncating requestfile to first {} lines".format(args.requestfile_truncate))

    # data structures shared by processes :
    queue = multiprocessing.Queue()  # type: multiprocessing.Queue[str]
    exhausted_flag = cast(c_bool, multiprocessing.Value(c_bool, False))  # used to signal end of feed

    # starting feeder/consumer processes :
    feeder = start_feeder(logger, queue, exhausted_flag, args.requestfile, args.requestfile_truncate)
    consumers = []
    result_pipes = []
    for i in range(args.num_client_processes):
        consumer, result_pipe = start_consumer(
            logger,
            args.num_client_coro,
            queue,
            exhausted_flag,
        )
        consumers.append(consumer)
        result_pipes.append(result_pipe)

    # handling results :
    aggregated, nb_success, nb_errors = aggregate_results(result_pipes)
    dump_results(logger, aggregated, nb_success, nb_errors, args.output)

    # joining consumers is probably not necessary as if aggregate_results returned, all recv have succeeded.
    # It means that all consumers have successfully sent (meaning that their processes properly exited)
    # and if consumers stopped looping, it's because exhausted_flag was set by feeder process (meaning
    # that feeder process properly exited).
    for consumer in consumers:
        consumer.join()
    feeder.join()
