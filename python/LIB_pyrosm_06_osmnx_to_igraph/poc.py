#!/usr/bin/env python3

import pyrosm
import osmnx as ox
import matplotlib.pyplot as plt
import networkx as nx
import igraph as ig


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


def id_to_osm_way_url(way_id):
    return "https://www.openstreetmap.org/way/{}".format(way_id)


def main():
    print("DESCRIPTION = convert an osmnx graph to igraph")
    print("")
    print("What I would like, ideally :")
    print("- given a node (resp. edge) in igraph, retrieve the equivalent node in osmnx (and vice-versa)")
    print("- given a node in igraph (resp. osmnx), retrieve the equivalent node in OSM data (and vice-versa)")
    print("- given an edge in igraph (resp. osmnx), retrieve its parent way in OSM data")
    print("- given an OSM way, retrieve all its edges in igraph/osmnx")
    print("")
    print("This POC merely scratches the surface, and creates an igraph Graph from an osmnx one.")

    # build igraph from PBF file, first using "driving" network_type :
    INPUT_PBF_FILE = "NOGIT_monaco.osm.pbf"
    minx = 7.417826056480407
    maxx = 7.420363426208495
    miny = 43.72635527380567
    maxy = 43.72775470745523

    osm = pyrosm.OSM(INPUT_PBF_FILE, bounding_box=[minx, miny, maxx, maxy])
    nodes, edges = osm.get_network(nodes=True, network_type="walking")
    G = osm.to_graph(nodes, edges, graph_type="networkx")

    # osmnx can't work with the "geometry" property, apparently...
    # cf. https://stackoverflow.com/questions/65568844/typeerror-unhashable-type-linestring-when-using-ox-simplify-graph/65592832#65592832  # noqa
    for _, _, d in G.edges(data=True):
        d.pop("geometry", None)

    G_simplified = ox.simplify_graph(G)
    fig, ax_simplified = ox.plot_graph(G_simplified, node_color="r")
    ax_simplified.set_title("simplified")
    plt.show()

    # see : https://github.com/gboeing/osmnx-examples/blob/master/notebooks/14-osmnx-to-igraph.ipynb
    osmids = list(G_simplified.nodes)
    G_simplified = nx.relabel.convert_node_labels_to_integers(G_simplified)

    # give each node its original osmid as attribute since we relabeled them
    osmid_values = {k: v for k, v in zip(G_simplified.nodes, osmids)}
    nx.set_node_attributes(G_simplified, osmid_values, "osmid")

    # convert to igraph :
    G_ig = ig.Graph(directed=True)
    G_ig.add_vertices(G_simplified.nodes)
    G_ig.add_edges(G_simplified.edges())
    G_ig.vs["osmid"] = osmids
    G_ig.es["length"] = list(nx.get_edge_attributes(G_simplified, "length").values())

    print("igraph G has {} vertices and {} edges".format(G_ig.vcount(), G_ig.ecount()))
    print("first 2 nodes are :")
    print(G_ig.vs[0])
    print(G_ig.vs[1])
    print("first 2 edges are :")
    print(G_ig.es[0])
    print(G_ig.es[1])


main()
