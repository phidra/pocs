#!/usr/bin/env python
# coding: utf-8

import sys

def _upper_one_over_two(input_string):
    should_upper = True
    for char in input_string:
        if should_upper:
            yield char.upper()
        else:
            yield char.lower()
        should_upper = not should_upper


def convert(input_string):
    return "".join(_upper_one_over_two(input_string))

if __name__ == "__main__":
    for arg in sys.argv[1:]:
        print "{} -> {}".format(arg, convert(arg))


