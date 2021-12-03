#!/usr/bin/env python3

import pyrosm
import igraph
import matplotlib


def main():
    print("DESCRIPTION = first test with pyrosm : loads + plots test graph, converts it into igraph object")
    print("PYROSM PATH =", pyrosm.__path__)
    print("IGRAPH PATH =", igraph.__path__)

    # Pyrosm comes with a couple of test datasets
    # that can be used straight away without
    # downloading anything
    fp = pyrosm.get_data("test_pbf")

    # Initialize the OSM parser object
    osm = pyrosm.OSM(fp)

    # Read all drivable roads :
    drive_network = osm.get_network(network_type="driving")
    drive_network.plot()
    matplotlib.pyplot.show()

    # convert to igraph :
    nodes, edges = osm.get_network(nodes=True, network_type="driving")
    g = osm.to_graph(nodes, edges)
    igraph.plot(g, "/tmp/pouet.png", layout=g.layout_kamada_kawai())

    first_edge = g.es[0]
    print("FIRST EDGE = ", first_edge)


main()
