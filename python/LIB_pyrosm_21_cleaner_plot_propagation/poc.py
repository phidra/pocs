#!/usr/bin/env python3

from pathlib import Path
import sys

import networkx as nx

from original_graph import OriginalGraph
from drawing import DijkstraDrawer, PlotConf
from dijkstra import Dijkstra


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


def main():

    output_dir = Path(sys.argv[1])
    print("Using output_dir = {}".format(output_dir))
    assert(output_dir.is_dir())

    # Load original graph :
    print("")
    extract_file_path = "NOGIT_monaco.osm.pbf"
    orig_graph = OriginalGraph.create_from_osm_extract(extract_file_path, polygon=None)
    print("Nb edges = ", orig_graph.graph.number_of_edges())
    print("Nb nodes = ", orig_graph.graph.number_of_nodes())
    G = orig_graph.graph

    # SOURCE :
    # https://www.openstreetmap.org/node/1866422978
    source_osmid = 1866422978
    print("SOURCE VERTEX IS : ", id_to_osm_node_url(source_osmid))

    # TARGET :
    # https://www.openstreetmap.org/node/1868736741
    target_osmid = 1868736741
    print("TARGET VERTEX IS : ", id_to_osm_node_url(target_osmid))

    # the "reference" shortest path is the networkx one :
    reference_path = nx.shortest_path(G, source=source_osmid, target=target_osmid, weight="length")
    print("NETWORKX shortest path goes through these nodes (first ones, at least) :")
    for idx, node in enumerate(reference_path):
        print("\t", node, " = ", id_to_osm_node_url(node))
        if idx > 8:
            break
    print("\t...")
    print("")

    drawer = DijkstraDrawer(output_dir)
    drawer.plot_graph(G, **PlotConf.NO_NODE, **PlotConf.LIGHT_EDGES)  # background graph
    dijkstra = Dijkstra(G, event_handler=drawer, weight_attr="length")
    path = dijkstra.run(source=source_osmid, target=target_osmid)
    print("MANUAL shortest path goes through these nodes (first ones, at least) :")
    for idx, node in enumerate(path):
        print("\t", node, " = ", id_to_osm_node_url(node))
        if idx > 8:
            break
    print("\t...")
    assert(path == reference_path)
    print("An assert ensures that the manual path is the expected path :-)")
    print("")


if __name__ == "__main__":
    main()
