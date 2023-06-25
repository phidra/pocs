#!/usr/bin/env python3

from pathlib import Path
import sys
import string
import itertools

import networkx as nx

from glifov.original_graph import OriginalGraph
from glifov.drawing import UnidirDijkstraDrawer, Conf as GraphConf
from glifov.dijkstra import Dijkstra
from glifov.annotation import annotate_all_edges, annotate_node, annotate_all_nodes, Conf


def load_original_graph(extract_file_path, polygon_file_path):
    polygon = OriginalGraph.polygon_from_file(polygon_file_path)
    print("Using POLYGON = ", polygon)
    orig_graph = OriginalGraph.create_from_osm_extract(extract_file_path, polygon)
    print("Nb edges = ", orig_graph.graph.number_of_edges())
    print("Nb nodes = ", orig_graph.graph.number_of_nodes())
    return orig_graph


def draw_reference_path(orig_graph, SOURCE, TARGET, node_to_text, output_dir):
    reference_path = nx.shortest_path(orig_graph.graph, source=SOURCE, target=TARGET, weight="length")
    print("NETWORKX shortest path goes through these nodes (first ones, at least) :")
    costs = []
    for tail, head in zip(reference_path[:-1], reference_path[1:]):
        cost = round(orig_graph.graph[tail][head][0]["length"])
        print("\t", tail, " = ", OriginalGraph.osmid_to_osmurl(tail), "   cost = ", cost)
        costs.append(cost)

    print("\t", head, " = ", OriginalGraph.osmid_to_osmurl(head))
    print("sum cost = ", cost)
    print("")
    drawer = UnidirDijkstraDrawer(output_dir, "path", 1, max_nb_file=1)
    drawer.plot_graph(
        orig_graph.graph,
        **GraphConf.LIGHT_NODES,
        **GraphConf.LIGHT_EDGES,
    )
    with drawer.keep_axes_limits():
        drawer.plot_graph(
            orig_graph.graph.subgraph(reference_path),
            **GraphConf.NO_NODE,
            **GraphConf.RED_EDGES,
        )
    annotate_all_nodes(orig_graph.graph, drawer.main_ax, excluded_nodes=(SOURCE, TARGET), node_to_text=node_to_text)
    annotate_node(orig_graph.graph, SOURCE, "SOURCE", drawer.main_ax, **Conf.VIP)
    annotate_node(orig_graph.graph, TARGET, "TARGET", drawer.main_ax, **Conf.VIP)
    annotate_all_edges(orig_graph.graph, drawer.main_ax, color="r")
    costs_str = "+".join(str(cost) for cost in costs)
    total_cost = sum(costs)
    texts = [
        f"SOURCE = {SOURCE}",
        f"TARGET = {TARGET}",
        f"costs = {costs_str}",
        f"total cost = {total_cost}",
    ]
    drawer.draw_temporary_text("\n".join(texts))
    drawer.save_figure()


def main():
    extract_file_path = Path(sys.argv[1])
    polygon_file_path = Path(sys.argv[2])
    output_dir = Path(sys.argv[3])
    print("reading OSM extract from :", extract_file_path)
    print("reading OSM polygon from :", polygon_file_path)
    print("dumping in output dir    :", output_dir)
    assert(output_dir.is_dir())

    orig_graph = load_original_graph(extract_file_path, polygon_file_path)

    SOURCE = 1868767862  # https://www.openstreetmap.org/node/1868767862
    TARGET = 826809711  # https://www.openstreetmap.org/node/826809711

    # first, draw the empty graph :
    drawer = UnidirDijkstraDrawer(output_dir, "dijkstra-propagation", 1, max_nb_file=25)
    drawer.plot_graph(
        orig_graph.graph,
        **GraphConf.LIGHT_NODES,
        **GraphConf.LIGHT_EDGES,
    )
    annotate_node(orig_graph.graph, SOURCE, "SOURCE", drawer.main_ax, **Conf.VIP)
    annotate_node(orig_graph.graph, TARGET, "TARGET", drawer.main_ax, **Conf.VIP)
    annotate_all_edges(orig_graph.graph, drawer.main_ax, color="r")

    node_names = itertools.cycle(string.ascii_uppercase)

    def node_to_text(*_, **__):
        nonlocal node_names
        return next(node_names)

    annotate_all_nodes(orig_graph.graph, drawer.main_ax, excluded_nodes=(SOURCE, TARGET), node_to_text=node_to_text)

    drawer.save_figure()
    drawer.begin_new_repetition()

    dijkstra = Dijkstra(orig_graph.graph, event_handler=drawer, weight_attr="length")
    inexisting_target = 99999  # we want the DRAWER to drive the stop of the algorithm
    dijkstra.run(source=SOURCE, target=inexisting_target)

    draw_reference_path(orig_graph, SOURCE, TARGET, node_to_text, output_dir)


if __name__ == "__main__":
    main()
