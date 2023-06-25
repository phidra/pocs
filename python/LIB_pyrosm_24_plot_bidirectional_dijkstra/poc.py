#!/usr/bin/env python3

from pathlib import Path
import sys

import networkx as nx

from original_graph import OriginalGraph
from drawing import DijkstraDrawer, PlotConf
from dijkstra import Dijkstra
from bidir_dijkstra import BidirDijkstra


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


class DummyDrawer:
    def on_node_settled(self, dijkstra, currently_settled_node):
        pass


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

    # unidirectional dijkstra :
    dummy_drawer = DummyDrawer()
    dijkstra = Dijkstra(G, event_handler=dummy_drawer, weight_attr="length")
    path1 = dijkstra.run(source=source_osmid, target=target_osmid)

    assert(path1 == reference_path)
    print("An assert ensures that monodijkstra path is the expected path :-)")
    print("")

    # drawer :
    drawer = DijkstraDrawer(output_dir, "image", save_period=10, max_nb_file=0)
    drawer.plot_graph(G, **PlotConf.NO_NODE, **PlotConf.LIGHT_EDGES)  # background graph

    # bidirectional dijkstra :
    print("Running bidir dijkstra")
    bidir_dijkstra = BidirDijkstra(
        G,
        G,
        event_handler_forward=drawer,
        event_handler_backward=drawer,
        weight_attr="length",
    )
    path2 = bidir_dijkstra.run(source=source_osmid, target=target_osmid)
    assert(path2 == reference_path)
    print("An assert ensures that bidir-ijkstra path is ALSO the expected path :-)")
    print("")


if __name__ == "__main__":
    main()
