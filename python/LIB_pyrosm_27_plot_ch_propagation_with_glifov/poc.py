#!/usr/bin/env python3

from pathlib import Path
import sys

import networkx as nx

from glifov.original_graph import OriginalGraph
from glifov.drawing import DijkstraDrawer, Conf
from glifov.dijkstra import Dijkstra
from glifov.bidir_dijkstra import BidirDijkstra
from glifov.ch import load_ch


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


class DummyDrawer:
    def on_node_settled(*_, **__):
        pass

    def on_forward_node_settled(*_, **__):
        pass

    def on_backward_node_settled(*_, **__):
        pass


def load_original_graph(extract_file_path, polygon_file_path):
    polygon = OriginalGraph.polygon_from_file(polygon_file_path)
    # FIXME : for now, we ignore polygon :
    polygon = None
    print("Using POLYGON = ", polygon)
    orig_graph = OriginalGraph.create_from_osm_extract(extract_file_path, polygon)
    print("Nb edges = ", orig_graph.graph.number_of_edges())
    print("Nb nodes = ", orig_graph.graph.number_of_nodes())
    return orig_graph


def get_source_target_and_reference_path(orig_graph):

    # SOURCE :
    # https://www.openstreetmap.org/node/1866422978
    source_osmid = 1866422978
    print("SOURCE VERTEX IS : ", id_to_osm_node_url(source_osmid))

    # TARGET :
    # https://www.openstreetmap.org/node/1868736741
    target_osmid = 1868736741
    print("TARGET VERTEX IS : ", id_to_osm_node_url(target_osmid))

    # the "reference" shortest path is the networkx one :
    reference_path = nx.shortest_path(orig_graph.graph, source=source_osmid, target=target_osmid, weight="length")
    print("NETWORKX shortest path goes through these nodes (first ones, at least) :")
    for idx, node in enumerate(reference_path):
        print("\t", node, " = ", id_to_osm_node_url(node))
        if idx > 8:
            break
    print("\t...")
    print("")
    return source_osmid, target_osmid, reference_path


def run_unidir_dijkstra(orig_graph, source_osmid, target_osmid):
    dummy_drawer = DummyDrawer()
    dijkstra = Dijkstra(orig_graph.graph, event_handler=dummy_drawer, weight_attr="length")
    path = dijkstra.run(source=source_osmid, target=target_osmid)
    return path


def run_bidir_dijkstra(orig_graph, source_osmid, target_osmid, output_dir):
    drawer = DijkstraDrawer(output_dir, "bidir", save_period=10, max_nb_file=0)
    drawer.plot_graph(orig_graph.graph, **Conf.NO_NODE, **Conf.LIGHT_EDGES)  # background graph
    bidir_dijkstra = BidirDijkstra(
        orig_graph.graph,
        orig_graph.graph,
        event_handler_forward=drawer,
        event_handler_backward=drawer,
        weight_attr="length",
    )
    forward_path, joining_node, backward_path = bidir_dijkstra.run(source=source_osmid, target=target_osmid)

    if not backward_path:
        return forward_path
    backward_path.reverse()
    path = forward_path + backward_path[1:]  # exclude one joining node
    return path


def run_ch_query(orig_graph, source_osmid, target_osmid, output_dir):
    ch, raw_ch = load_ch(orig_graph, output_dir)
    print(raw_ch.summary())

    drawer = DijkstraDrawer(output_dir, "ch", save_period=1, max_nb_file=0)
    drawer.plot_graph(orig_graph.graph, **Conf.NO_NODE, **Conf.LIGHT_EDGES)  # background graph

    bidir_dijkstra = BidirDijkstra(
        ch.forward,
        ch.backward,
        event_handler_forward=drawer,
        event_handler_backward=drawer,
        weight_attr="length",
    )
    forward_path, joining_node, backward_path = bidir_dijkstra.run(source=source_osmid, target=target_osmid)

    # unpacking forward_path :
    regular_path_forward = [source_osmid]
    for contracted_tail, contracted_head in zip(forward_path[:-1], forward_path[1:]):
        contracted_edge = ch.forward[contracted_tail][contracted_head][0]
        regular_path_forward += contracted_edge["unpacked_path"][1:]

    # unpacking backward path :
    regular_path_backward = []
    for contracted_tail, contracted_head in zip(backward_path[:-1], backward_path[1:]):
        contracted_edge = ch.backward[contracted_tail][contracted_head][0]
        regular_path_backward += reversed(contracted_edge["unpacked_path"][1:])

    # TODO : pérenniser ceci...
    # FIXME : et expliquer... parce qu'il y a plein de confusions à faire, et elles se mélangent toutes :
    #   - forward/backward
    #   - nodes dupliqués à enlever (joining-node / source / target / premier-node de chaque raccourcis, etc.)
    #   - inversion de chemins à faire
    #   - ...
    # En clair, j'ai tatonné à partir d'exemples pour retrouverl a bonne reconstruction
    # (et je ne suis même pas sûr que ce serait correct sir le graphe n'était pas symétrique !)
    if not regular_path_backward:
        return regular_path_forward
    regular_path_backward.reverse()  # FIXME = describe
    return regular_path_forward + regular_path_backward


def main():
    extract_file_path = Path(sys.argv[1])
    polygon_file_path = Path(sys.argv[2])
    output_dir = Path(sys.argv[3])
    print("reading OSM extract from :", extract_file_path)
    print("reading OSM polygon from :", polygon_file_path)
    print("dumping in output dir    :", output_dir)
    assert(output_dir.is_dir())

    orig_graph = load_original_graph(extract_file_path, polygon_file_path)
    source_osmid, target_osmid, reference_path = get_source_target_and_reference_path(orig_graph)

    # unidirectional dijkstra :
    unidir_path = run_unidir_dijkstra(orig_graph, source_osmid, target_osmid)

    # bidirectional dijkstra :
    bidir_path = run_bidir_dijkstra(orig_graph, source_osmid, target_osmid, output_dir)

    # CH :
    ch_path = run_ch_query(orig_graph, source_osmid, target_osmid, output_dir)

    assert(unidir_path == reference_path)
    assert(bidir_path == reference_path)
    assert(ch_path == reference_path)
    print("Some asserts ensure that all path are identical to the reference path :-)")
    print("")


if __name__ == "__main__":
    main()
