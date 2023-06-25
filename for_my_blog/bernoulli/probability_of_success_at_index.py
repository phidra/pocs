#!/usr/bin/env python3.6

from random import randint
import sys
import argparse
from typing import Tuple


def run_bernoulli_trial(p: int) -> bool:
    """
    precondition:  p ∈ [1;100]
    p/100 is Bernoulli parameter
    returns True if Bernoulli trial was a success
    """
    value = randint(1, 100)  # value ∈ [1;100]
    return value <= p


def run_until_count(p: int, count: int) -> bool:
    """
    Run exactly 'count' Bernoulli trials (with parameter p/100).
    Returns True if there was at least a success, False otherwise.
    precondition:  p ∈ [1;100]
    """
    for _ in range(count):
        if run_bernoulli_trial(p):
            return True
    return False


def parse() -> Tuple[int, int]:
    parser = argparse.ArgumentParser(
        description="Compute the probability of (at least) one success after a given number of trial.",
    )
    parser.add_argument(
        "parameter",
        type=int,
        help="Bernoulli parameter, an integer belonging to [1;100]",
    )
    parser.add_argument(
        "count",
        type=int,
        help="count for which we want the probability, belonging to [1;+∞[",
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

    if args.count <= 0:
        raise ValueError("count must be > 0")

    nb_of_runs = args.runs
    if nb_of_runs <= 0:
        raise ValueError("Nb of runs must be > 0")

    print(f"Bernoulli parameter = {p}")
    print(f"Desired count = {args.count}")
    print(f"Number of runs = {nb_of_runs}")
    return nb_of_runs, p, args.count


def main():
    nb_of_runs, p, count = parse()

    accumulation = 0
    for i in range(nb_of_runs):
        status = run_until_count(p, count)
        accumulation += int(status)
    measured_probability_of_success = accumulation / nb_of_runs

    probability_of_failure = (100 - p) / 100
    theoretical_probability_of_success = 1 - (probability_of_failure**count)
    print(f"probability of (at least) one success at count {count} is :")
    print(f"\tmeasured    = {measured_probability_of_success}")
    print(f"\ttheoretical = {theoretical_probability_of_success}")


main()
