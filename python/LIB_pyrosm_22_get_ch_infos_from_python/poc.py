#!/usr/bin/env python3

import sys
from pathlib import Path

import matplotlib.pyplot as plt
import osmnx

from original_graph import OriginalGraph
from ch_info_dumper_wrapper import build_ch
from ch import ContractionHierarchy


if __name__ == "__main__":
    extract_file_path = Path(sys.argv[1])
    polygon_file_path = Path(sys.argv[2])
    output_dir = Path(sys.argv[3])
    print("reading OSM extract from :", extract_file_path)
    print("reading OSM polygon from :", polygon_file_path)
    print("dumping in output dir    :", output_dir)

    polygon = OriginalGraph.polygon_from_file(polygon_file_path)
    print("Using POLYGON = ", polygon)

    print("")
    orig_graph = OriginalGraph.create_from_osm_extract(extract_file_path, polygon)
    print("Nb edges = ", orig_graph.graph.number_of_edges())
    print("Nb nodes = ", orig_graph.graph.number_of_nodes())

    # (debug) displaying the order of some nodes :
    counter = 0
    DEBUG_DISPLAY_LIMIT = 5
    for node_osmid, node_props in orig_graph.graph.nodes(data=True):
        print("The node with osmid {} has order {}".format(node_props["osmid"], node_props["order"]))
        counter += 1
        if counter > DEBUG_DISPLAY_LIMIT:
            break

    graph_files = orig_graph.dump_files(Path.cwd())
    print("Edges are in these files :")
    print("    -", graph_files.tailsfile)
    print("    -", graph_files.headsfile)
    print("    -", graph_files.weightsfile)
    print("Node's osmids are in this file :")
    print("    -", graph_files.osmidsfile)

    print("")
    print("")
    print("")
    raw_ch = build_ch(graph_files, output_dir)
    print("CALL TO CH INFOS DUMPER MADE :")
    print(raw_ch.summary())
    print("")

    ch = ContractionHierarchy.create(raw_ch, orig_graph)
    ch.conversion._check_consistency()

    fig, ax = plt.subplots(figsize=(12.80, 10.24))
    fig.tight_layout(pad=0, h_pad=0, w_pad=0)
    edge_colors = osmnx.plot.get_edge_colors_by_attr(ch.forward, attr="is_shortcut", num_bins=2, cmap="coolwarm")
    osmnx.plot_graph(
        ch.forward,
        ax=ax,
        bgcolor="k",
        node_size=0,
        edge_color=edge_colors,
        edge_linewidth=2,
        show=False,
    )
    filename = "/tmp/pouet.png"
    fig.savefig(filename)
    print("BASE GRAPH DUMPED IN : ", filename)
