#!/usr/bin/env python3

from pathlib import Path
import sys

from glifov.original_graph import OriginalGraph
from glifov.ch import load_ch
from glifov.drawing import IncrementalDrawer, Conf as GraphConf
from glifov.annotation import Conf as AnnotationConf, annotate_node
from glifov.vrac_a_classer import (
    linearring_to_geojson,
    get_osmnx_bbox,
    out_edges_bbox,
    out_edges_subgraph,
)


def load_original_graph(extract_file_path, polygon_file_path):
    polygon = OriginalGraph.polygon_from_file(polygon_file_path)
    # FIXME : for now, we ignore polygon :
    polygon = None
    print("Using POLYGON = ", polygon)
    orig_graph = OriginalGraph.create_from_osm_extract(extract_file_path, polygon)
    print("Nb edges = ", orig_graph.graph.number_of_edges())
    print("Nb nodes = ", orig_graph.graph.number_of_nodes())
    return orig_graph


def annotate_relevant_nodes(
    original_graph,
    ch,
    original_out_edges,
    contracted_out_edges,
    osmid,
    ax,
    with_minor,
    with_major,
):
    """ annotates relevant nodes = successors in the original graph and in the contracted graph """

    def annotate_main_node(node):
        rank = ch.conversion.osmid_to_rank[node]
        return annotate_node(original_graph.graph, node, str(rank), ax, **AnnotationConf.VIP)

    def annotate_upward_node(node):
        rank = ch.conversion.osmid_to_rank[node]
        return annotate_node(original_graph.graph, node, str(rank), ax, **AnnotationConf.MAJOR)

    def annotate_downward_node(node):
        rank = ch.conversion.osmid_to_rank[node]
        return annotate_node(original_graph.graph, node, str(rank), ax, **AnnotationConf.MINOR)

    upward_nodes = set()
    downward_nodes = set()

    for node in original_out_edges.nodes():
        downward_nodes.add(node)

    for tail, head, edge_data in contracted_out_edges.edges(data=True):
        upward_nodes.add(tail)
        upward_nodes.add(head)
        for n in edge_data["unpacked_path"]:
            downward_nodes.add(n)

    # if a node belongs to several categories, some categories have higher priority :  MAIN > UPWARD > DOWNWARD
    main_node_set = set([osmid])
    remaining_upward = upward_nodes.difference(main_node_set)

    # do the annotation :
    annotate_main_node(osmid)

    # EDIT : this has become quite an unreadable mess to choose what do draw according to with_major/with_minor...
    if with_major:
        for n in remaining_upward:
            annotate_upward_node(n)

    if with_minor:
        for n in original_graph.graph:
            if n not in main_node_set and (not with_major or n not in remaining_upward):
                annotate_downward_node(n)


def draw_node_analysis_image(osmid, original_graph, ch, output_dir, filename, with_minor, with_major):
    shapely_bbox_original = out_edges_bbox(original_graph.graph, osmid)
    shapely_bbox_contracted = out_edges_bbox(ch.forward, osmid)
    merged_bbox = shapely_bbox_original.union(shapely_bbox_contracted)

    print("=== ANALYSIS BBOX IS :")
    print(linearring_to_geojson(merged_bbox.exterior))
    print("")

    drawer = IncrementalDrawer(output_dir, filename, 1)
    drawer.plot_graph(
        original_graph.graph,
        **GraphConf.LIGHT_NODES,
        **GraphConf.LIGHT_EDGES,
        bbox=get_osmnx_bbox(merged_bbox),
    )

    original_out_edges_graph = out_edges_subgraph(original_graph.graph, osmid)
    contracted_out_edges_graph = out_edges_subgraph(ch.forward, osmid)

    with drawer.keep_axes_limits():
        if with_minor:
            drawer.plot_graph(original_out_edges_graph, edge_color="r", edge_linewidth=6, **GraphConf.NO_NODE)
        if with_major:
            drawer.plot_graph(contracted_out_edges_graph, edge_color="b", edge_linewidth=2, **GraphConf.BIGBLUE_NODES)

    annotate_relevant_nodes(
        original_graph,
        ch,
        original_out_edges_graph,
        contracted_out_edges_graph,
        osmid,
        drawer.main_ax,
        with_minor=with_minor,
        with_major=with_major,
    )

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
    ch, raw_ch = load_ch(orig_graph, output_dir)

    interesting_to_plot = [
        1072,
        1034,
        1068,
        1015,
        1012,
        1027,
        1026,
        1035,
        1014,
    ]

    for node_rank in interesting_to_plot:
        node_osmid = ch.conversion.rank_to_osmid[node_rank]
        print("Analyzing node : ", OriginalGraph.osmid_to_osmurl(node_osmid))
        f = f"poc28-image-rank{node_rank}-node{node_osmid}"
        draw_node_analysis_image(node_osmid, orig_graph, ch, output_dir, f, with_minor=True, with_major=True)
        draw_node_analysis_image(node_osmid, orig_graph, ch, output_dir, f + "-bis", with_minor=False, with_major=True)
        draw_node_analysis_image(node_osmid, orig_graph, ch, output_dir, f + "-ter", with_minor=True, with_major=False)


if __name__ == "__main__":
    main()
