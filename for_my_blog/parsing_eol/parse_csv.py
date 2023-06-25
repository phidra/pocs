#!/usr/bin/env python3

import csv
import sys
from functools import reduce

if len(sys.argv) <= 1:
    raise RuntimeError("Missing file(s) to parse")

filenames = sys.argv[1:]
parsings = []
for filename in filenames:
    print("About to parse: {}".format(filename))
    with open(filename, "r") as f:
        reader = csv.reader(f)
        parsing = []

        for tokens in reader:
            print("\tNEW LINE :")
            this_line = []
            for token in tokens:
                print("\t\t '{}'".format(token))
                this_line.append(token)
            parsing.append(this_line)
        print("---end---")
    parsings.append(parsing)

all_files_were_parsed_identically = reduce(lambda a, b: a == b, parsings)
if not all_files_were_parsed_identically:
    print("WARNING : input files were parsed differently !")
