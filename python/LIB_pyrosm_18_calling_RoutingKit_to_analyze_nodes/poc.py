#!/usr/bin/env python3

import subprocess
from dataclasses import dataclass, field
from typing import List, Tuple
from contextlib import contextmanager

import pyrosm
import osmnx as ox
import matplotlib.pyplot as plt
import networkx as nx
from shapely.geometry import MultiPoint, Polygon, LinearRing, box


@contextmanager
def keep_axes_limits(ax):
    xlim_before_second_drawing = ax.get_xlim()
    ylim_before_second_drawing = ax.get_ylim()
    try:
        yield ax
    finally:
        ax.set_xlim(xlim_before_second_drawing)
        ax.set_ylim(ylim_before_second_drawing)


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


@dataclass
class DumpedGraph:
    node_array: List[int]
    tailsfile: str
    headsfile: str
    weightsfile: str
    osmidsfile: str

    @property
    def num_nodes(self):
        return len(self.node_array)


def step1_read_graph():

    # STEP 1 = building graph (no filtering here) :
    INPUT_PBF_FILE = "NOGIT_monaco.osm.pbf"
    print("")
    print("=== STEP 1 = reading graph from {}".format(INPUT_PBF_FILE))

    osm = pyrosm.OSM(INPUT_PBF_FILE)
    nodes, edges = osm.get_network(nodes=True, network_type="walking")
    G_nonsimplified = osm.to_graph(nodes, edges, graph_type="networkx")

    # osmnx can't work with the "geometry" property, apparently...
    # cf. https://stackoverflow.com/questions/65568844/typeerror-unhashable-type-linestring-when-using-ox-simplify-graph/65592832#65592832  # noqa
    for _, _, d in G_nonsimplified.edges(data=True):
        d.pop("geometry", None)

    G = ox.simplify_graph(G_nonsimplified)
    return G


def step2_dump_graph(G) -> DumpedGraph:

    print("")
    print("=== STEP 2 = dumping graph")

    # attributing a rkindex to each node :
    # rk = RoutingKit (= the id by which RoutingKit will know the node) :
    node_rkindex = 0
    node_array = []
    for node_osmid, node_props in G.nodes(data=True):
        node_props["rkindex"] = node_rkindex
        node_array.append(node_osmid)
        node_rkindex += 1

    num_nodes = len(node_array)
    assert num_nodes == node_rkindex
    print("There are {} nodes in the graph".format(num_nodes))

    # (debug) displaying the rkindex of some nodes :
    counter = 0
    for node_osmid, node_props in G.nodes(data=True):
        counter += 1
        # (debug) displaying the rkindex of som nodes :
        if counter <= 5:
            print(
                "The node with osmid {} has rkindex {}".format(
                    node_props["osmid"], node_props["rkindex"]
                )
            )

    # dumping edges in a format usable by RoutingKit :
    tails = []
    heads = []
    weights = []
    for edge_tail, edge_head, edge_props in G.edges(data=True):
        # the graph is a multigraph : each nodepair (head, tail) might have SEVERAL edges.
        # but when we iterate over G.edges like that, it seem that only one edge at a time is returned.
        tails.append(G.nodes[edge_tail]["rkindex"])
        heads.append(G.nodes[edge_head]["rkindex"])
        weights.append(edge_props["length"])

    assert len(tails) == len(heads)
    assert len(tails) == len(weights)

    print("")
    print("Dumping files :")
    headsfile = "NOGIT_heads.txt"
    tailsfile = "NOGIT_tails.txt"
    weightsfile = "NOGIT_weights.txt"
    osmidsfile = "NOGIT_osmids.txt"

    with open(tailsfile, "w") as f_tails:
        print("Dumping {} tails to : {}".format(len(tails), tailsfile))
        f_tails.write("\n".join(str(x) for x in tails))
        f_tails.write("\n")

    with open(headsfile, "w") as f_heads:
        print("Dumping {} heads to : {}".format(len(heads), headsfile))
        f_heads.write("\n".join(str(x) for x in heads))
        f_heads.write("\n")

    with open(weightsfile, "w") as f_weights:
        print("Dumping {} weights to : {}".format(len(weights), weightsfile))
        f_weights.write("\n".join(str(x) for x in weights))
        f_weights.write("\n")

    with open(osmidsfile, "w") as f_osmids:
        print("Dumping {} osmids to : {}".format(len(node_array), osmidsfile))
        f_osmids.write("\n".join(str(x) for x in node_array))
        f_osmids.write("\n")

    return DumpedGraph(node_array, tailsfile, headsfile, weightsfile, osmidsfile)


@dataclass
class NodeInfo:
    rkindex: int
    rank: int
    osmid: int
    osmurl: str
    original_successors: List["NodeInfo"] = field(default_factory=list)
    contracted_successors: List["ContractedNodeInfo"] = field(default_factory=list)

    @staticmethod
    def from_fields(fields: str) -> "NodeInfo":
        tokens = fields.split(" ")
        rkindex = int(tokens[0])
        rank = int(tokens[1])
        osmid = int(tokens[2])
        osmurl = tokens[3]
        return NodeInfo(rkindex, rank, osmid, osmurl)

    def __str__(self):
        return "{osmid} (rkindex={rkindex} rank={rank}) {osmurl}".format(
            osmid=self.osmid,
            rkindex=self.rkindex,
            rank=self.rank,
            osmurl=self.osmurl,
        )


@dataclass
class ContractedNodeInfo(NodeInfo):
    shortcut_nodes: List[int] = field(default_factory=list)  # nodes are identified by their rkindex

    @staticmethod
    def from_fields(fields: str) -> "ContractedNodeInfo":
        tokens = fields.split(" ")
        rkindex = int(tokens[0])
        rank = int(tokens[1])
        osmid = int(tokens[2])
        osmurl = tokens[3]
        shortcut_nodes = [int(x) for x in tokens[4:]]
        return ContractedNodeInfo(rkindex, rank, osmid, osmurl, [], [], shortcut_nodes)

    def print(self):
        print(str(self))
        for n in self.shortcut_nodes:
            print(n)


def rk_analyze_node(g: DumpedGraph, node_rkindex: int) -> Tuple[List[int], NodeInfo]:
    # each node has TWO identifiers :
    #   osmid = the node id in the OSM data
    #   rkindex = the index of the node in the node-array
    #
    # The only nodes that RoutingKit is aware of are rkindex.

    BINARY = "./NOGIT_node_analyzer"
    ENV = {
        "LD_LIBRARY_PATH": "DEPENDENCY_node_analyzer_with_RoutingKit/NOGIT_RoutingKit_repository/lib",
    }
    args = [
        BINARY,
        g.tailsfile,
        g.headsfile,
        g.weightsfile,
        g.osmidsfile,
        str(node_rkindex),
    ]

    print("About to call RoutingKit node-analyzing binary with these args :")
    for arg in args:
        print("\t", arg)
    print("")

    completed_process = subprocess.run(
        args, capture_output=True, check=True, text=True, env=ENV
    )

    print("Logs of the call :")
    print(completed_process.stderr)
    print("")

    # the node info are written on stdout :
    # LINE 1 = number of nodes
    # LINE 2 = rank of each node (ordered by node id), as a space-separated list of ranks
    # LINE 3 = node info, as space-separated fields :  <rkindex rank osmid osmurl>
    # LINE 4 = node's ORIGINAL successor nodes (same field as above, each group being separated by '|')
    # LINE 5 = node's CONTRACTED successor nodes (additional fields contains the rkindices of the unpacked shortcut)

    output = filter(None, completed_process.stdout.split("\n"))

    num_nodes = int(next(output))
    print("num nodes = ", num_nodes)
    node_ranks = [int(n) for n in next(output).split(" ") if n]
    print("num ranks = ", len(node_ranks))

    node_info_line = next(output)
    node_info = NodeInfo.from_fields(node_info_line)

    original_successors_line = next(output)
    for original_successor_fields in filter(None, original_successors_line.split("|")):
        node_info.original_successors.append(NodeInfo.from_fields(original_successor_fields))

    contracted_successors_line = next(output)
    for contracted_successor_fields in filter(None, contracted_successors_line.split("|")):
        node_info.contracted_successors.append(ContractedNodeInfo.from_fields(contracted_successor_fields))

    return node_ranks, node_info


def step3_compute_shortest_path_long(G, dumped_graph: DumpedGraph) -> Tuple[List[int], NodeInfo]:

    node_osmid = 263077002  # https://www.openstreetmap.org/node/263077002
    node_rkindex = G.nodes[node_osmid]["rkindex"]
    node_osmurl = id_to_osm_node_url(node_osmid)
    print("We are interested in node {} (rkindex={}) : {}".format(node_osmid, node_rkindex, node_osmurl))
    return rk_analyze_node(dumped_graph, node_rkindex)


def annotate_node(G, ranks: List[int], node_osmid: int, ax):
    """ Given a node's osmid, annotate it on the figure with is rkindex """
    point = G.nodes[node_osmid]["x"], G.nodes[node_osmid]["y"]
    rkindex = G.nodes[node_osmid]["rkindex"]
    rank = ranks[rkindex]
    ax.annotate(
        str(rank),
        point,
        textcoords="offset pixels",
        xytext=(11, 9),  # annotations are slightly shifted from the annotated node
        fontweight="bold",
        color="w",
        backgroundcolor=(0.5, 0.5, 0.5, 0.2)  # slightliy translucide,
    )
    ax.annotate(
        str(rank),
        point,
        textcoords="offset pixels",
        xytext=(10, 10),  # annotations are slightly shifted from the annotated node
        color="k",
        fontweight="bold",
    )


def coords_of_osmid(G, osmid: int) -> Tuple[float, float]:
    return G.nodes[osmid]["x"], G.nodes[osmid]["y"]


def coords_of_node_info(G, node_info: NodeInfo) -> Tuple[float, float]:
    return coords_of_osmid(G, node_info.osmid)


def bbox_extended_by_fraction(geom, fraction=0.1) -> Polygon:
    """ From a geometry, retourns a bbox that surrounds it, extended by a fraction of the original bbox """
    min_x, min_y, max_x, max_y = geom.bounds

    delta_x = max_x - min_x
    ten_percent_x = delta_x * fraction
    extended_min_x = min_x - ten_percent_x
    extended_max_x = max_x + ten_percent_x

    delta_y = max_y - min_y
    ten_percent_y = delta_y * fraction
    extended_min_y = min_y - ten_percent_y
    extended_max_y = max_y + ten_percent_y
    ten_percent_y = delta_y * fraction

    return box(extended_min_x, extended_min_y, extended_max_x, extended_max_y)


def to_geojson(polygon_exterior: LinearRing) -> str:
    polygon_coords = []
    for x, y in polygon_exterior.coords:
        polygon_coords.append("[{}, {}]".format(x, y))
    polygon_coords_str = ",".join(polygon_coords)

    BEFORE = """
{
  "type": "FeatureCollection",
  "features": [
    {
      "type": "Feature",
      "properties": {},
      "geometry": {
        "type": "Polygon",
        "coordinates": [
            [
    """
    AFTER = """
            ]
            ]
          }
        }
      ]
    }
    """

    return BEFORE + polygon_coords_str + AFTER


def draw_subgraph(ax, G, nodes, annotated_nodes, edges, edge_color, edge_linewidth, bbox):
    subgraph = nx.MultiDiGraph(**G.graph)
    for node in nodes:
        subgraph.add_node(node, **G.nodes[node])
    for tail, head in edges:
        for _, edge_data in G.adj[tail][head].items():
            subgraph.add_edge(tail, head, **edge_data)

    with keep_axes_limits(ax):
        ox.plot_graph(
            subgraph,
            ax=ax,
            node_size=0,
            edge_color=edge_color,
            edge_linewidth=edge_linewidth,
            show=False,
            bbox=bbox,
        )

    for node in annotated_nodes:
        subgraph = nx.MultiDiGraph(**G.graph)
        subgraph.add_node(node, **G.nodes[node])
        with keep_axes_limits(ax):
            ox.plot_graph(
                subgraph,
                ax=ax,
                node_size=60,
                node_color="g",
                edge_linewidth=0,
                node_edgecolor="b",
                show=False,
                bbox=bbox,
            )

    for node in nodes:
        annotate_node(G, ranks, node, ax)


def step4_dump_image(G, dumped_graph: DumpedGraph, ranks: List[int], node_info: NodeInfo, filename: str):

    # === STEP 1 = find a proper bbox :
    # the bbox includes the node, and its neighbours (contracted+original) :
    points = []
    points.append(coords_of_node_info(G, node_info))
    for neighbour in node_info.original_successors:
        points.append(coords_of_node_info(G, neighbour))
    for neighbour in node_info.contracted_successors:
        points.append(coords_of_node_info(G, neighbour))
    multipoint = MultiPoint(points)

    print("=== EXTENDED BBOX IS :")
    shapely_bbox = bbox_extended_by_fraction(multipoint.envelope)
    print(to_geojson(shapely_bbox.exterior))
    print("")

    # === STEP 2 = draw the whole graph :
    fig, ax = plt.subplots(figsize=(12.80, 10.24))
    fig.tight_layout(pad=0, h_pad=0, w_pad=0)
    west, south, east, north = shapely_bbox.bounds
    bbox = (north, south, east, west)
    ox.plot_graph(
        G,
        ax=ax,
        bgcolor="k",
        node_size=0,
        edge_color="k",
        edge_linewidth=0.5,
        show=False,
        bbox=bbox,
    )

    # === STEP 3 = draw a small graph that only contains the original neighbours :
    tail = node_info.osmid
    nodes = set([tail])
    edges = []
    for neigh in node_info.original_successors:
        head = neigh.osmid
        nodes.add(head)
        edges.append((tail, head))
    draw_subgraph(ax, G, nodes, nodes, edges, "r", 5, bbox)

    # === STEP 4 = draw a small graph that only contains the contracted neighbours :
    tail = node_info.osmid
    nodes = set([tail])
    annotated_nodes = set([tail])
    edges = []
    for contracted_neighbour in node_info.contracted_successors:
        predecessor = tail
        annotated_nodes.add(contracted_neighbour.osmid)
        for node_on_shortcut_rkindex in contracted_neighbour.shortcut_nodes:
            node_on_shortcut_osmid = dumped_graph.node_array[node_on_shortcut_rkindex]
            nodes.add(node_on_shortcut_osmid)
            edges.append((predecessor, node_on_shortcut_osmid))
            predecessor = node_on_shortcut_osmid
    draw_subgraph(ax, G, nodes, annotated_nodes, edges, "b", 2, bbox)

    # === STEP 4 = save the figure :
    fig.savefig(filename)
    print("BASE GRAPH DUMPED IN : ", filename)


if __name__ == "__main__":
    G = step1_read_graph()
    dumped_graph = step2_dump_graph(G)
    ranks, node_info = step3_compute_shortest_path_long(G, dumped_graph)
    print("ANALYZED NODE = {}".format(node_info))
    print("Its ORIGINAL successors :")
    for succ in node_info.original_successors:
        print("\t{}".format(succ))
    print("Its CONTRACTED successors :")
    for succ in node_info.contracted_successors:
        print("\t", succ)
        for n_rkindex in succ.shortcut_nodes:
            n_osmid = dumped_graph.node_array[n_rkindex]
            n_rank = ranks[n_rkindex]
            print("\t\t RANK={}  {}".format(n_rank, id_to_osm_node_url(n_osmid)))
    step4_dump_image(G, dumped_graph, ranks, node_info, "/tmp/graph18.png")
