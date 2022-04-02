#!/usr/bin/env python3

from pathlib import Path

import osmium


class CountHandler(osmium.SimpleHandler):
    def __init__(self):
        osmium.SimpleHandler.__init__(self)
        self.num_nodes = 0
        self.num_ways = 0

    def node(self, n):
        self.num_nodes += 1

    def way(self, w):
        self.num_ways += 1


def main() -> None:
    print("POC DESCRIPTION = basic usage of pyosmium to parse osm file")
    datafile = Path(__file__).parent / "DATA_single_edge.osm"
    myhandler = CountHandler()
    myhandler.apply_file(datafile)
    print(f"NUMBER OF NODES = {myhandler.num_nodes}")
    print(f"NUMBER OF WAYS = {myhandler.num_ways}")
    assert myhandler.num_nodes == 5
    assert myhandler.num_ways == 1


if __name__ == "__main__":
    main()
