#!/usr/bin/env python3

from process import run
from pathlib import Path

testdir = Path("../testcases/").resolve()
for input_path in testdir.glob("input.*"):
    expected_path = testdir / ("output" + input_path.suffix)
    print(input_path, end="")
    with open(str(expected_path), "r") as expected:
        got_lines = run(str(input_path))
        expected_lines = [x.rstrip("\n") for x in expected.readlines() if not x.startswith("#")]
        if got_lines != expected_lines:
            print("... ERROR")
            print("GOT :")
            for line in got_lines:
                print(line)
            print("EXPECTED :")
            for line in expected_lines:
                print(line)

        assert got_lines == expected_lines
        print(" ... OK")
