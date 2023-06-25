#!/usr/bin/env python3

from pathlib import Path
import sys

from original_graph import OriginalGraph
from drawing import DijkstraDrawer, PlotConf
from dijkstra import Dijkstra
from ch_info_dumper_wrapper import build_ch
from ch import ContractionHierarchy


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


def main():
    extract_file_path = Path(sys.argv[1])
    polygon_file_path = Path(sys.argv[2])
    output_dir = Path(sys.argv[3])
    print("reading OSM extract from :", extract_file_path)
    print("reading OSM polygon from :", polygon_file_path)
    print("dumping in output dir    :", output_dir)
    assert(output_dir.is_dir())

    # STEP 1 = load graph :
    polygon = OriginalGraph.polygon_from_file(polygon_file_path)
    polygon = None  # ignoring polygon for now
    print("Using POLYGON = ", polygon)
    orig_graph = OriginalGraph.create_from_osm_extract(extract_file_path, None)
    print("Nb edges = ", orig_graph.graph.number_of_edges())
    print("Nb nodes = ", orig_graph.graph.number_of_nodes())

    # STEP 2 = load CH :
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

    # https://www.openstreetmap.org/node/1097219387
    origin_osmid = 1097219387
    print("ORIGIN VERTEX IS : ", id_to_osm_node_url(origin_osmid))

    # note : this bbox is DIFFERENT from the filtering bbox !
    # drawing_bbox = 43.730065058221406, 43.72490153386983, 7.422423362731933, 7.414097785949707  # N, S, E, W
    drawing_bbox = None
    drawer = DijkstraDrawer(output_dir, "image", save_period=1, max_nb_file=0, drawing_bbox=drawing_bbox)
    drawer.plot_graph(ch.forward, **PlotConf.NO_NODE, **PlotConf.LIGHT_EDGES)  # background graph
    dijkstra = Dijkstra(ch.forward, event_handler=drawer, weight_attr="length")
    dijkstra.run(source=origin_osmid, target=dijkstra.NO_TARGET)


if __name__ == "__main__":
    main()
