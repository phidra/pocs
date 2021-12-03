#!/usr/bin/env python3

import pyrosm
import osmnx as ox
import matplotlib.pyplot as plt


def main():
    print("DESCRIPTION = more advanced plottings")

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
    ax.set_title("simplified")

    # these edges are to be coloured in red :
    #   https://www.openstreetmap.org/way/517029019
    #   https://www.openstreetmap.org/way/849737751
    # moreover, we color in blue the way that have a name :
    def color_edge(data):
        if data["osmid"] in (517029019, 849737751):
            return "r"
        if "name" in data:
            return "b"
        return "k"

    edge_color = [color_edge(d) for (_, _, d) in G.edges(data=True)]

    # these nodes are to be coloured in green :
    #   https://www.openstreetmap.org/node/1704462737
    #   https://www.openstreetmap.org/node/4452216587
    #   https://www.openstreetmap.org/node/4452216591
    #   https://www.openstreetmap.org/node/263077303
    def color_node(data):
        if data["osmid"] in (
            1704462737,
            4452216587,
            4452216591,
            263077303,
        ):
            return "r"
        return "y"

    node_color = [color_node(d) for (_, d) in G.nodes(data=True)]

    ox.plot_graph(G, ax=ax, bgcolor="k", node_color=node_color, edge_color=edge_color, edge_linewidth=0.8, show=False)

    already_annotated_ways = set()

    for _, _, d in G.edges(data=True):
        # to simplify, we ignore elementary edges
        if "geometry" not in d:
            continue
        way_name = d["name"]
        centroid = d["geometry"].centroid
        if way_name and way_name not in already_annotated_ways:
            already_annotated_ways.add(way_name)
            print("new way name = ", way_name)
            ax.annotate(way_name, (centroid.x, centroid.y), c="k")

    for node_index, (_, d) in enumerate(G.nodes(data=True)):
        position = d["geometry"]
        ax.annotate(node_index, (position.x, position.y), c="k")

    # just for fun (ok, ok and also to prove that geographic coordinates are used in plot), here is a big triangle :
    ax.scatter(
        y=[43.72717],
        x=[7.41868],
        s=530,
        c="r",
        alpha=0.8,
        marker="^",
    )

    # apparently, graph was plotted edges first, then nodes :
    fig.legend([f"nb edges = {G.size()}", f"nb nodes = {len(G)}"])

    dumped = "/tmp/graph09.png"
    fig.savefig(dumped)
    print("DUMPED GRAPH IN : ", dumped)


main()
