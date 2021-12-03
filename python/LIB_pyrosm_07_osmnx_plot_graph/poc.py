#!/usr/bin/env python3

import pyrosm
import osmnx as ox
import matplotlib.pyplot as plt


def main():
    print("DESCRIPTION = diverse plottings + display edge content")

    # build from PBF file, first using "driving" network_type :
    INPUT_PBF_FILE = "NOGIT_monaco.osm.pbf"
    minx = 7.417826056480407
    maxx = 7.420363426208495
    miny = 43.72635527380567
    maxy = 43.72775470745523

    osm = pyrosm.OSM(INPUT_PBF_FILE, bounding_box=[minx, miny, maxx, maxy])
    nodes, edges = osm.get_network(nodes=True, network_type="walking")
    G_nonsimplified = osm.to_graph(nodes, edges, graph_type="networkx")

    # osmnx can't work with the "geometry" property, apparently...
    # cf. https://stackoverflow.com/questions/65568844/typeerror-unhashable-type-linestring-when-using-ox-simplify-graph/65592832#65592832  # noqa
    for _, _, d in G_nonsimplified.edges(data=True):
        d.pop("geometry", None)

    G = ox.simplify_graph(G_nonsimplified)

    fig, ax = plt.subplots()
    ax.set_title("Non-simplified")

    # automatically color nodes according to their latitude :
    node_color = ox.plot.get_node_colors_by_attr(G, attr="y", cmap="plasma")

    # automatically coloring the edges according to their length
    NUM_BINS = 2  # only two lengths : BIG or SMALL
    edge_color = ox.plot.get_edge_colors_by_attr(G, attr="length", num_bins=NUM_BINS, cmap="coolwarm")
    ox.plot_graph(G, ax=ax, node_color=node_color, edge_color=edge_color, bgcolor="k", edge_linewidth=0.3, show=False)

    # displaying a route on a graph :
    route = [
        1875148246,
        1875148225,
        1875148204,
        1875148200,
        1704462781,
    ]
    # https://www.openstreetmap.org/node/1875148246
    # https://www.openstreetmap.org/node/1875148225
    # https://www.openstreetmap.org/node/1875148204
    # https://www.openstreetmap.org/node/1875148200
    # https://www.openstreetmap.org/node/1704462781

    # https://osmnx.readthedocs.io/en/stable/osmnx.html#osmnx.plot.plot_graph_route
    ox.plot_graph_route(G, ax=ax, show=False, route=route, route_linewidth=3, route_alpha=0.7, route_color="r")

    dumped = "/tmp/graph07.png"
    fig.savefig(dumped)
    print("DUMPED GRAPH IN : ", dumped)


main()
