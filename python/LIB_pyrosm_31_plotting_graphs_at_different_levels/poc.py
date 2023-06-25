#!/usr/bin/env python3

from pathlib import Path
import sys

from glifov.original_graph import OriginalGraph
from glifov.ch import load_ch
from glifov.vrac_a_classer import get_osmnx_bbox
from glifov.drawing import IncrementalDrawer, Conf as GraphConf


def load_original_graph(extract_file_path):
    orig_graph = OriginalGraph.create_from_osm_extract(extract_file_path, None)
    print("Nb edges = ", orig_graph.graph.number_of_edges())
    print("Nb nodes = ", orig_graph.graph.number_of_nodes())
    return orig_graph


def draw_level(level: int, ch, output_dir, bbox):
    nodes_to_plot = set()
    for node in ch.forward.nodes():
        if ch.conversion.osmid_to_rank[node] >= level:
            nodes_to_plot.add(node)

    subgraph = ch.forward.subgraph(nodes_to_plot)
    drawer = IncrementalDrawer(output_dir, f"level-{level}", 1)
    drawer.plot_graph(
        subgraph,
        node_size=10,
        node_color="r",
        node_zorder=0,
        **GraphConf.LIGHT_EDGES,
        bbox=bbox,
    )

    text = f"CH-LEVEL = {level}"
    text += f"\nnb nodes remaining = {subgraph.number_of_nodes()}"
    drawer.draw_temporary_text(text)
    drawer.save_figure()
    drawer.begin_new_repetition()


def main():
    extract_file_path = Path(sys.argv[1])
    polygon_file_path = Path(sys.argv[2])
    output_dir = Path(sys.argv[3])
    print("reading OSM extract from :", extract_file_path)
    print("reading OSM polygon from :", polygon_file_path)
    print("dumping in output dir    :", output_dir)
    assert(output_dir.is_dir())

    polygon = OriginalGraph.polygon_from_file(polygon_file_path)
    bbox = get_osmnx_bbox(polygon)

    orig_graph = load_original_graph(extract_file_path)
    ch, raw_ch = load_ch(orig_graph, output_dir)

    interesting_levels = [
        1,
        10,
        20,
        50,
        100,
        500,
        1000,
        1500,
        2000,
        2020,
        2050,
        2100,
        2150,
        2200,
        orig_graph.graph.number_of_nodes() - 30,
        orig_graph.graph.number_of_nodes() - 20,
        orig_graph.graph.number_of_nodes() - 10,
        orig_graph.graph.number_of_nodes() - 5,
    ]
    for level in interesting_levels:
        draw_level(level, ch, output_dir, bbox)


if __name__ == "__main__":
    main()
