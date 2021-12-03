#!/usr/bin/env python3

import pyrosm
import osmnx as ox
import networkx as nx
import matplotlib.pyplot as plt
from functools import partial


def _is_in_range(value, range_):
    min_, max_ = range_
    return value >= min_ and value <= max_


def _is_in_bbox(point, long_range, lat_range):
    return _is_in_range(point.x, long_range) and _is_in_range(point.y, lat_range)


def get_subgraph_first_way_of_doing_it(G, long_range, lat_range):
    """
    returns a subgraph with only the nodes/edegs in a bbox
    this uses subgraph_view
    """
    is_relaxed = partial(_is_in_bbox, long_range=long_range, lat_range=lat_range)

    def filter_node(n):
        return is_relaxed(G.nodes[n]["geometry"])

    def filter_edge(u, v, _):
        edge_data = G.adj[u][v]
        if "geometry" in edge_data:
            position = edge_data["geometry"].centroid
            return is_relaxed(position)
        else:
            return is_relaxed(G.nodes[u]["geometry"]) and is_relaxed(G.nodes[v]["geometry"])

    return nx.subgraph_view(G, filter_node=filter_node, filter_edge=filter_edge)


def get_subgraph_second_way_of_doing_it(G, long_range, lat_range):
    """
    returns a subgraph with only the nodes/edegs in a bbox
    this first mark the nodes, then build the subgraph using a given list
    """
    # first, marking the nodes and edges :
    is_relaxed = partial(_is_in_bbox, long_range=long_range, lat_range=lat_range)
    for u, v, d in G.edges(data=True):
        if "geometry" in d:
            position = d["geometry"].centroid
            d["marked"] = is_relaxed(position)
        else:
            d["marked"] = is_relaxed(G.nodes[u]["geometry"]) and is_relaxed(G.nodes[v]["geometry"])

    for _, d in G.nodes(data=True):
        position = d["geometry"]
        d["marked"] = is_relaxed(position)

    # second, building subgraph from a list of marked nodes :
    inducing_nodes = [n for (n, marked) in G.nodes(data="marked") if marked]
    return G.subgraph(nodes=inducing_nodes)


def main():
    print("DESCRIPTION = using a carefully crafted subgraph to plot differently some edges/nodes")

    # STEP 1 = building + filtering graph :
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

    # Among others, this node and this way will be drawn specifically :
    #   https://www.openstreetmap.org/node/4452216587
    #   https://www.openstreetmap.org/way/448186733

    # STEP 2 = drawing a first plot of the graph :
    ox.plot_graph(G, ax=ax, bgcolor="k", node_color="r", edge_color="k", edge_linewidth=0.8, show=False)

    # STEP 3 = over the first drawing, draw a second subgraph :

    # WARNING : unless 'bbox' param is passed, plot_graph will adjust the limits to the SECOND (smaller) dataset.
    # solution 1 = pass 'bbox' arg to the second drawing :
    # solution 2 = memorize limits BEFORE the second drawing, and set them again AFTER the second drawing (used here)
    xlim_before_second_drawing = ax.get_xlim()
    ylim_before_second_drawing = ax.get_ylim()
    LONG_RANGE_1 = 7.419092059135436, 7.420164942741393
    LAT_RANGE_1 = 43.72631263126983, 43.7270530564435
    G2 = get_subgraph_first_way_of_doing_it(G, LONG_RANGE_1, LAT_RANGE_1)
    ox.plot_graph(G2, ax=ax, bgcolor="k", node_size=100, node_color="g", edge_linewidth=2, edge_color="g", show=False)
    ax.set_xlim(xlim_before_second_drawing)
    ax.set_ylim(ylim_before_second_drawing)

    # STEP 4 = over the first two drawings, draw a third subgraph :
    xlim_before_third_drawing = ax.get_xlim()
    ylim_before_third_drawing = ax.get_ylim()
    LONG_RANGE_2 = 7.418378591537476, 7.419210076332092
    LAT_RANGE_2 = 43.727390315137974, 43.7279950501108
    G3 = get_subgraph_second_way_of_doing_it(G, LONG_RANGE_2, LAT_RANGE_2)
    ox.plot_graph(G3, ax=ax, bgcolor="k", node_size=100, node_color="y", edge_linewidth=2, edge_color="y", show=False)
    ax.set_xlim(xlim_before_third_drawing)
    ax.set_ylim(ylim_before_third_drawing)

    dumped = "/tmp/graph10.png"
    fig.savefig(dumped)
    print("DUMPED GRAPH IN : ", dumped)


main()
