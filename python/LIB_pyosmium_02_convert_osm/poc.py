#!/usr/bin/env python3

import sys
from pathlib import Path

import osmium


class Converter(osmium.SimpleHandler):
    def __init__(self, writer):
        super(Converter, self).__init__()
        self.writer = writer

    def node(self, n):
        self.writer.add_node(n)

    def way(self, w):
        self.writer.add_way(w)


def main() -> None:
    print("POC DESCRIPTION = basic usage of pyosmium to convert file.osm to file.osm.pbf")
    input_file_osm = sys.argv[1]
    output_file_osmpbf = sys.argv[2]
    print(f"INPUT  = {input_file_osm}")
    print(f"OUTPUT = {output_file_osmpbf}")

    Path(output_file_osmpbf).unlink(missing_ok=True)
    writer = osmium.SimpleWriter(str(output_file_osmpbf))
    handler = Converter(writer)
    handler.apply_file(input_file_osm)
    writer.close()


if __name__ == "__main__":
    main()
