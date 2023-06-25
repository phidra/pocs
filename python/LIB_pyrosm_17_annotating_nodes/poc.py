#!/usr/bin/env python3

import subprocess
from dataclasses import dataclass
from typing import List, Tuple
from contextlib import contextmanager

import pyrosm
import osmnx as ox
import matplotlib.pyplot as plt
import networkx as nx

import route_long


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


def step2_dump_graph(G):

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

    return DumpedGraph(node_array, tailsfile, headsfile, weightsfile)


def rk_shortest_path(
    g: DumpedGraph, source_rkindex: int, target_rkindex: int
) -> Tuple[List[int], int, List[int], List[int]]:
    # each node has TWO identifiers :
    #   osmid = the node id in the OSM data
    #   rkindex = the index of the node in the node-array
    #
    # The only nodes that RoutingKit is aware of are rkindex.

    BINARY = "./NOGIT_shortest_path_calculator"
    ENV = {
        "LD_LIBRARY_PATH": "DEPENDENCY_shortest_path_calculator_with_RoutingKit/NOGIT_RoutingKit_repository/lib",
    }
    args = [
        BINARY,
        g.tailsfile,
        g.headsfile,
        g.weightsfile,
        str(g.num_nodes),
        str(source_rkindex),
        str(target_rkindex),
    ]

    print("About to call RoutingKit shortest-path binary with these arges :")
    for arg in args:
        print("\t", arg)
    print("")

    completed_process = subprocess.run(
        args, capture_output=True, check=True, text=True, env=ENV
    )

    print("Logs of the call :")
    print(completed_process.stderr)
    print("")

    # the shortest path and its length are written on stdout (length = first line, nodes = other lines)
    parsed_output = filter(None, completed_process.stdout.split("\n"))
    node_ranks = [int(n) for n in next(parsed_output).split(" ") if n]
    shortest_path_length = int(next(parsed_output))
    shortest_path_nodes_rkindex = [int(n) for n in next(parsed_output).split(" ") if n]
    print("Node ranks = ", node_ranks)
    print("Shortest-path length = ", shortest_path_length)
    print("Shortest-path nodes = ", shortest_path_nodes_rkindex)
    print("")

    # converting shortest path from rkindex to osmid :
    shortest_path_nodes_osmid = [g.node_array[x] for x in shortest_path_nodes_rkindex]
    return node_ranks, shortest_path_length, shortest_path_nodes_rkindex, shortest_path_nodes_osmid


def step3_compute_shortest_path_long(
    G, dumped_graph: DumpedGraph
) -> Tuple[List[int], List[int], int, int]:

    # ROUTE1 = LONG DISTANCE :
    source_rkindex_long = G.nodes[route_long.SOURCE_OSMID]["rkindex"]
    target_rkindex_long = G.nodes[route_long.TARGET_OSMID]["rkindex"]
    print("On route1 (long distance) we have :")
    print(
        "    SOURCE as osmid    = {}".format(
            id_to_osm_node_url(route_long.SOURCE_OSMID)
        )
    )
    print("    SOURCE as rk index = {}".format(source_rkindex_long))
    print(
        "    TARGET as osmid    = {}".format(
            id_to_osm_node_url(route_long.TARGET_OSMID)
        )
    )
    print("    TARGET as rk index = {}".format(target_rkindex_long))

    ranks, length, path_rkindex, path_osmid = rk_shortest_path(
        dumped_graph, source_rkindex_long, target_rkindex_long
    )

    # À noter qu'ici en l'état, je ne peux pas comparer avec le chemin attendu.
    #
    # En effet, pour faire le chemin entre ces deux nodes :
    #   https://www.openstreetmap.org/node/5046302792
    #   https://www.openstreetmap.org/node/7781960117
    #
    # ...le chemin attendu emprunte ce node :
    #   https://www.openstreetmap.org/node/7781960118
    #
    # ...alors que le chemin renvoyé par RoutingKit emprunte plutôt les nodes suivants :
    #   https://www.openstreetmap.org/node/25177285
    #   https://www.openstreetmap.org/node/5route2_long918147594
    #   https://www.openstreetmap.org/node/25177282
    #
    # En pratique, les chemins étant très proche, on est sans doute dans l'erreur d'arrondi.
    # En effet, je tronque les weights flottant en les convertissant en entiers (mécaniquement, ça réduit leur longueur)

    return ranks, path_osmid, route_long.SOURCE_OSMID, route_long.TARGET_OSMID


def annotate_node(G, ranks: List[int], node_osmid: int, ax):
    """ Given a node's osmid, annotate it on the figure with is rkindex """
    point = G.nodes[node_osmid]["x"], G.nodes[node_osmid]["y"]
    rkindex = G.nodes[node_osmid]["rkindex"]
    rank = ranks[rkindex]
    ax.annotate(
        str(rank),
        point,
        textcoords="offset pixels",
        xytext=(10, 10),  # annotations are slightly shifted from the annotated node
        fontweight="bold",
        backgroundcolor=(0.5, 0.5, 0.5, 0.2)  # slightliy translucide,
    )


def step4_dump_image(
        G, ranks: List[int], path_osmid: List[int], origin: int, destination: int, filename: str
):
    # small bbox around origin :
    NORTH = 43.728332303501425
    SOUTH = 43.726204086496104
    EAST = 7.420658469200133
    WEST = 7.417075037956237
    BBOX = (NORTH, SOUTH, EAST, WEST)

    # === STEP 1 = draw the whole graph :
    fig, ax = plt.subplots(figsize=(12.80, 10.24))
    fig.tight_layout(pad=0, h_pad=0, w_pad=0)
    ox.plot_graph(
        G,
        ax=ax,
        bgcolor="k",
        node_size=0,
        edge_color="k",
        edge_linewidth=0.5,
        show=False,
        bbox=BBOX,
    )

    # === STEP 2 = builds a small graph that only contains the shortest path and draw it :
    path_graph = nx.MultiDiGraph(**G.graph)
    first_node = path_osmid[0]
    path_graph.add_node(
        first_node, **G.nodes[first_node]
    )  # here, we add the first tail

    # adding all needed nodes/edges :
    for tail, head in zip(path_osmid[:-1], path_osmid[1:]):
        # we only add the head of each edge (and the first tail has already been added) :
        path_graph.add_node(head, **G.nodes[head])
        for _, edge_data in G.adj[tail][head].items():
            path_graph.add_edge(tail, head, **edge_data)

    with keep_axes_limits(ax):
        ox.plot_graph(
            path_graph,
            ax=ax,
            node_size=0,
            edge_color="r",
            edge_linewidth=3,
            show=False,
            bbox=BBOX,
        )

    # === STEP 3 = builds a small graph that only contains the origin and destination nodes + draw it :
    od_graph = nx.MultiDiGraph(**G.graph)
    od_graph.add_node(origin, **G.nodes[origin])
    od_graph.add_node(destination, **G.nodes[destination])
    with keep_axes_limits(ax):
        # node_size seems to be in another unit than pixels...
        ox.plot_graph(
            od_graph,
            ax=ax,
            node_size=60,
            node_color="g",
            edge_linewidth=0,
            node_edgecolor="b",
            show=False,
            bbox=BBOX,
        )

    for node_osmid in G.nodes(data=False):
        annotate_node(G, ranks, node_osmid, ax)

    # === STEP 4 = save the figure :
    fig.savefig(filename)
    print("BASE GRAPH DUMPED IN : ", filename)


if __name__ == "__main__":
    G = step1_read_graph()
    dumped_graph = step2_dump_graph(G)
    ranks, path_osmid, origin, destination = step3_compute_shortest_path_long(G, dumped_graph)
    step4_dump_image(G, ranks, path_osmid, origin, destination, "/tmp/graph17.png")
