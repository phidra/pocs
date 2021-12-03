#!/usr/bin/env python3

import pyrosm
import osmnx as ox
import matplotlib.pyplot as plt


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


def id_to_osm_way_url(way_id):
    return "https://www.openstreetmap.org/way/{}".format(way_id)


def main():
    print("DESCRIPTION = simplifies a given graph using osmnx")

    # build from PBF file, first using "driving" network_type :
    INPUT_PBF_FILE = "NOGIT_monaco.osm.pbf"
    minx = 7.417826056480407
    maxx = 7.420363426208495
    miny = 43.72635527380567
    maxy = 43.72775470745523

    osm = pyrosm.OSM(INPUT_PBF_FILE, bounding_box=[minx, miny, maxx, maxy])
    nodes, edges = osm.get_network(nodes=True, network_type="walking")
    G = osm.to_graph(nodes, edges, graph_type="networkx")
    fig, axs = plt.subplots(2)
    fig, ax_nonsimplified = ox.plot_graph(G, ax=axs[0], node_color="r", show=False, close=False)
    ax_nonsimplified.set_title("Non-simplified")

    # osmnx can't work with the "geometry" property, apparently...
    # cf. https://stackoverflow.com/questions/65568844/typeerror-unhashable-type-linestring-when-using-ox-simplify-graph/65592832#65592832  # noqa
    for _, _, d in G.edges(data=True):
        d.pop("geometry", None)

    G_simplified = ox.simplify_graph(G)
    fig, ax_simplified = ox.plot_graph(G_simplified, ax=axs[1], node_color="r")
    ax_simplified.set_title("simplified")
    plt.show()


main()
