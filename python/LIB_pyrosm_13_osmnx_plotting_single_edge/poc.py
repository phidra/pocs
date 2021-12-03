#!/usr/bin/env python3

import pyrosm
import osmnx as ox
import networkx as nx
import matplotlib.pyplot as plt
from pprint import pprint


def main():
    print("DESCRIPTION = plotting a single edge")

    # STEP 1 = building graph (no filtering here) :
    INPUT_PBF_FILE = "NOGIT_monaco.osm.pbf"

    osm = pyrosm.OSM(INPUT_PBF_FILE)
    nodes, edges = osm.get_network(nodes=True, network_type="walking")
    G_nonsimplified = osm.to_graph(nodes, edges, graph_type="networkx")

    # osmnx can't work with the "geometry" property, apparently...
    # cf. https://stackoverflow.com/questions/65568844/typeerror-unhashable-type-linestring-when-using-ox-simplify-graph/65592832#65592832  # noqa
    for _, _, d in G_nonsimplified.edges(data=True):
        d.pop("geometry", None)

    G = ox.simplify_graph(G_nonsimplified)

    fig, ax = plt.subplots(figsize=(12.80, 10.24))  # 1280x1024 pixels, with the default 100dpi resolution
    size_in_pixels = fig.get_size_inches() * fig.dpi
    print("SIZE INCHES = ", fig.get_size_inches())
    print("SIZE DPI = ", fig.dpi)
    print("FIGSIZE = ", size_in_pixels)
    print("type(FIGSIZE) = ", type(size_in_pixels).__name__)  # ndarray

    # STEP 2 = base-plot : whole graph :
    ox.plot_graph(G, ax=ax, bgcolor="k", node_color="g", node_size=3, edge_color="k", edge_linewidth=0.8, show=False)

    # STEP 3 = we will plot the biggest out-edge of this node :
    # https://www.openstreetmap.org/node/1868736741
    INTERESTING_NODE = 1868736741
    successors = G.adj[INTERESTING_NODE]
    max_length = 0
    max_successor = -1
    max_data = {}
    for successor, multidata in successors.items():
        print("")
        print("There are ", len(multidata), "parallel out-edges with successor = ", successor)
        for idx, data in multidata.items():
            if data["length"] > max_length:
                max_length = data["length"]
                max_successor = successor
                max_data = data
            pprint(data)
            print("---")
    print("")
    print("The biggest successor of", INTERESTING_NODE, "is : ", max_successor, " (with length =", max_length, ")")

    print("")
    # just to see that graph class is MultiDiGraph
    print("type = ", type(G).__name__)
    # seeing the graph properties :
    # {'crs': 'EPSG:4326', 'name': 'Made with Pyrosm library.', 'simplified': True}
    pprint(G.graph)
    print("")

    # STEP 4 = building a minimalistic graph with a single edge (the one we want to plot) :
    # we keep the graph+nodes+edge properties :
    single_edge_graph = nx.MultiDiGraph(**G.graph)
    single_edge_graph.add_node(INTERESTING_NODE, **G.nodes[INTERESTING_NODE])
    single_edge_graph.add_node(max_successor, **G.nodes[max_successor])
    single_edge_graph.add_edge(INTERESTING_NODE, max_successor, **max_data)

    xlim_before_second_drawing = ax.get_xlim()
    ylim_before_second_drawing = ax.get_ylim()

    # tighter margins :
    fig.tight_layout(pad=0, h_pad=0, w_pad=0)

    ox.plot_graph(single_edge_graph, ax=ax, bgcolor="k", node_color="b", edge_color="r", edge_linewidth=3, show=False)
    ax.set_xlim(xlim_before_second_drawing)
    ax.set_ylim(ylim_before_second_drawing)

    dumped = "/tmp/graph13.png"
    fig.savefig(dumped)
    print("DUMPED GRAPH IN : ", dumped)


main()
