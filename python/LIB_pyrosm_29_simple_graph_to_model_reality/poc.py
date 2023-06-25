#!/usr/bin/env python3

from pathlib import Path
import sys

from glifov.original_graph import OriginalGraph
from glifov.ch import load_ch
from glifov.drawing import IncrementalDrawer, Conf as GraphConf
from glifov.vrac_a_classer import get_osmnx_bbox
from glifov.annotation import (
    Conf as AnnotationConf,
    annotate_all_nodes,
    annotate_all_edges,
    Conf as AnnotConf,
    annotate_node,
)


def load_original_graph(extract_file_path):
    orig_graph = OriginalGraph.create_from_osm_extract(extract_file_path, None)
    print("Nb edges = ", orig_graph.graph.number_of_edges())
    print("Nb nodes = ", orig_graph.graph.number_of_nodes())
    return orig_graph


def main():
    extract_file_path = Path(sys.argv[1])
    polygon_file_path = Path(sys.argv[2])
    output_dir = Path(sys.argv[3])
    print("reading OSM extract from :", extract_file_path)
    print("reading OSM polygon from :", polygon_file_path)
    print("dumping in output dir    :", output_dir)
    assert(output_dir.is_dir())

    orig_graph = load_original_graph(extract_file_path)
    ch, raw_ch = load_ch(orig_graph, output_dir)

    bbox = get_osmnx_bbox(OriginalGraph.polygon_from_file(polygon_file_path))

    drawer = IncrementalDrawer(output_dir, "modeling-reality", 1)
    drawer.plot_graph(
        orig_graph.graph,
        **GraphConf.LIGHT_NODES,
        **GraphConf.LIGHT_EDGES,
        bbox=bbox,
    )

    # first, draw the empty graph :
    drawer.save_figure()
    drawer.begin_new_repetition()

    # then draw only the nodes :
    annotate_all_nodes(orig_graph.graph, drawer.get_temp_ax(), **AnnotationConf.MINOR)
    drawer.save_figure()
    drawer.begin_new_repetition()

    # then draw only the edges :
    annotate_all_edges(orig_graph.graph, drawer.get_temp_ax(), color="r")
    drawer.save_figure()
    drawer.begin_new_repetition()

    # then draw nodes + edges :
    annotate_all_nodes(orig_graph.graph, drawer.get_temp_ax(), **AnnotationConf.MINOR)
    annotate_all_edges(orig_graph.graph, drawer.get_temp_ax(), color="r")
    drawer.save_figure()
    drawer.begin_new_repetition()

    # then draw ordering :

    def node_to_text(node, *_, **__):
        return str(ch.conversion.osmid_to_rank[node])

    options = AnnotationConf.MINOR.copy()
    options["color"] = "b"
    annotate_all_nodes(orig_graph.graph, drawer.get_temp_ax(), node_to_text=node_to_text, **options)
    SOURCE = 1868767862  # https://www.openstreetmap.org/node/1868767862
    options = AnnotConf.VIP.copy()
    options["zorder"] = 200
    annotate_node(orig_graph.graph, SOURCE, "SOURCE", drawer.main_ax, **options)
    drawer.save_figure()
    drawer.begin_new_repetition()


if __name__ == "__main__":
    main()
