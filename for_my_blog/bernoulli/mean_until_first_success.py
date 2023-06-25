#!/usr/bin/env python3.6

from random import randint
import sys
import argparse
from typing import Tuple


def run_bernoulli_trial(p: int) -> bool:
    """
    precondition:  p ∈ [1;100]
    p/100 is Bernoulli parameter
    """
    value = randint(1, 100)  # value ∈ [1;100]
    return value <= p


def run_until_success(p: int) -> int:
    """
    Repeatedly run Bernoulli trials (with parameter p/100), until first success.
    Returns the number of trials N that have been necessary to get it, including the succeeding one.
    precondition:  p ∈ [1;100]
    postcondition:  N ∈ [1;+∞[
    """
    counter = 0
    while True:
        counter += 1
        if run_bernoulli_trial(p):
            return counter


def parse() -> Tuple[int, int]:
    parser = argparse.ArgumentParser(
        description="Compute mean number of Bernoulli trials needed to get first success.",
    )
    parser.add_argument(
        "parameter",
        type=int,
        help="Bernoulli parameter, an integer belonging to [1;100]",
    )
    parser.add_argument(
        "--runs",
        type=int,
        default=10000,
        help="Nb of runs over which to compute mean (default: %(default)s)",
    )
    args = parser.parse_args()

    p = args.parameter  # bernoulli parameter
    if p < 0 or p > 100:
        raise ValueError("p is not in [1;100]")

    nb_of_runs = args.runs
    if nb_of_runs <= 0:
        raise ValueError("Nb of runs must be > 0")
    print(f"Bernoulli parameter = {p}")
    print(f"Number of runs = {nb_of_runs}")
    return nb_of_runs, p


def main():
    nb_of_runs, p = parse()

    accumulation = 0
    for i in range(nb_of_runs):
        first_success = run_until_success(p)
        accumulation += first_success
    print(f"mean = {accumulation / nb_of_runs}")


main()
