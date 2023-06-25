#!/usr/bin/env python3

from typing import Dict, Tuple, List, Any, TYPE_CHECKING
import ujson

if TYPE_CHECKING:
    import logging
    import pathlib
    import multiprocessing

DEFAULT_MAX_DISPLAY = 20


def aggregate_results(
    result_pipes: List["multiprocessing.connection.Connection"],
) -> Tuple[List[Dict[str, Any]], int, int]:
    """
    This mutates the input aggregated to append results to it.
    """
    aggregated: List[Dict[str, Any]] = []
    total_success = 0
    total_errors = 0

    for result_pipe in result_pipes:
        results, nb_success, nb_errors = result_pipe.recv()
        total_success += nb_success
        total_errors += nb_errors
        for url, status, response in results:
            aggregated.append(
                {
                    "url": url,
                    "status": status,
                    "response": response,
                }
            )

    return aggregated, total_success, total_errors


def dump_results(
    logger: "logging.Logger",
    aggregated: List[Dict[str, Any]],
    nb_success: int,
    nb_errors: int,
    dump_file: "pathlib.Path",
    max_display: int = DEFAULT_MAX_DISPLAY,
):
    nb_requests = nb_success + nb_errors
    logger.info("Total nb of requests = {}".format(nb_requests))
    logger.info("Total nb of success = {}".format(nb_success))

    with dump_file.open("w") as f:
        results = {
            "results": aggregated,
            "nb_requests": nb_requests,
            "nb_success": nb_success,
            "nb_errors": nb_errors,
        }
        f.write(ujson.dumps(results, indent=4))
