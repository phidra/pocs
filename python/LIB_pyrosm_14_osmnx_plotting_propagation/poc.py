#!/usr/bin/env python3

from pathlib import Path
import sys
import pyrosm
import osmnx as ox
import networkx as nx
from heapq import heappush, heappop
import matplotlib.pyplot as plt
from matplotlib.axes import Axes
from matplotlib.figure import Figure
from typing import Tuple


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


def plot_nodes(output_dir, fig, ax, G, parents, nodes, counter):
    """
    Given a bunch of nodes, plot their parent edges on an existing axe.
    A node's parent edge is the last edge on the shortest path from source to node.
    """

    # building a minimalistic graph with all the nodes and their parents :
    minimalistic_graph = nx.MultiDiGraph(**G.graph)
    for node in nodes:
        u = node  # the settled node
        v = parents[u]  # its parent
        if u == v:
            # guard (notably to handle source node)
            continue
        minimalistic_graph.add_node(u, **G.nodes[u])
        minimalistic_graph.add_node(v, **G.nodes[v])
        for _, edge_data in G.adj[u][v].items():
            minimalistic_graph.add_edge(u, v, **edge_data)

    xlim_before_second_drawing = ax.get_xlim()
    ylim_before_second_drawing = ax.get_ylim()
    ox.plot_graph(minimalistic_graph, ax=ax, bgcolor="k", node_size=0, edge_color="r", edge_linewidth=3, show=False)
    ax.set_xlim(xlim_before_second_drawing)
    ax.set_ylim(ylim_before_second_drawing)

    dumped = output_dir / "graph14-{:04d}.png".format(counter)
    fig.savefig(dumped)
    print("ADDING EDGE AND DUMPING IN : ", dumped)
    return counter + 1


def manual_dijkstra(output_dir, fig, ax, G, source, target, weight):
    costs = {}
    for node in G.nodes():
        costs[node] = sys.maxsize  # initial costs are huge
    costs[source] = 0  # source cost is null

    settled_nodes = set()
    parents = {source: source}
    queue = []
    heappush(queue, (0, source))

    counter = 1

    def relax(G, relaxed):
        relaxed_cost = costs[relaxed]
        for successor, multiedge_properties in G[relaxed].items():
            if successor in settled_nodes:
                continue
            edge_data = multiedge_properties[0]  # for the sake of simplicity, we ignore parallel edges

            successor_old_cost = costs[successor]
            successor_new_cost = relaxed_cost + edge_data[weight]

            if successor_new_cost < successor_old_cost:
                costs[successor] = successor_new_cost
                parents[successor] = relaxed
                heappush(queue, (successor_new_cost, successor))
                # note : this might insert duplicates, which is ok as all but one (the one with the smallest weight)
                # will be later ignored as already settled

    def compute_path(source, target, parents):
        path = []  # initially, reversed

        parent = target
        while parent != source:
            path.append(parent)
            parent = parents[parent]
        path.append(source)
        path.reverse()
        return path

    settled = source
    nodes_to_plot = []
    while queue and settled != target:
        _, settled = heappop(queue)
        settled_nodes.add(settled)
        relax(G, settled)
        nodes_to_plot.append(settled)
        if len(nodes_to_plot) > 10:
            counter = plot_nodes(output_dir, fig, ax, G, parents, nodes_to_plot, counter)
            nodes_to_plot = []

    path = compute_path(source, target, parents)
    return path


def step1_get_graph() -> nx.MultiDiGraph:
    # STEP 1 = building graph (no filtering here) :
    INPUT_PBF_FILE = "NOGIT_monaco.osm.pbf"

    osm = pyrosm.OSM(INPUT_PBF_FILE)
    nodes, edges = osm.get_network(nodes=True, network_type="walking")
    G_nonsimplified = osm.to_graph(nodes, edges, graph_type="networkx")

    # osmnx can't work with the "geometry" property, apparently...
    # cf. https://stackoverflow.com/questions/65568844/typeerror-unhashable-type-linestring-when-using-ox-simplify-graph/65592832#65592832  # noqa
    for _, _, d in G_nonsimplified.edges(data=True):
        d.pop("geometry", None)

    G = ox.simplify_graph(G_nonsimplified)
    return G


def step2_draw_base_graph(output_dir: Path, G: nx.MultiDiGraph) -> Tuple[Figure, Axes]:
    fig, ax = plt.subplots(figsize=(12.80, 10.24))
    fig.tight_layout(pad=0, h_pad=0, w_pad=0)
    size_in_pixels = fig.get_size_inches() * fig.dpi
    print("SIZE INCHES = ", fig.get_size_inches())
    print("SIZE DPI = ", fig.dpi)
    print("FIGSIZE = ", size_in_pixels)
    print("type(FIGSIZE) = ", type(size_in_pixels).__name__)  # ndarray
    ox.plot_graph(G, ax=ax, bgcolor="k", node_size=0, edge_color="k", edge_linewidth=0.5, show=False)

    dumped = output_dir / "graph14-0000.png"
    fig.savefig(dumped)
    print("BASE GRAPH DUMPED IN : ", dumped)
    return fig, ax


def main():
    print("DESCRIPTION = plotting dijkstra propagation")
    print("")
    print("This POC plots the propagation of dijkstra algorithm, in order to produce a video.")
    print("")
    print("Once images are generated, here are two commands possibles to convert them in a video :")
    print("    ffmpeg -f image2 -i 'graph14-%04d.png' -an -vcodec libx264 -crf 22 -threads 4 propagation1.avi")
    print('    mencoder "mf://graph14-*.png" -mf type=png:fps=50 -ovc lavc -lavcopts vcodec=mpeg4 -o propagation2.avi')

    output_dir = Path(sys.argv[1])
    print("Using output_dir = {}".format(output_dir))
    assert output_dir.is_dir()

    G = step1_get_graph()

    fig, ax = step2_draw_base_graph(output_dir, G)

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

    path = manual_dijkstra(output_dir, fig, ax, G, source=source_osmid, target=target_osmid, weight="length")
    print("MANUAL shortest path goes through these nodes (first ones, at least) :")
    for idx, node in enumerate(path):
        print("\t", node, " = ", id_to_osm_node_url(node))
        if idx > 8:
            break
    print("\t...")
    assert path == reference_path
    print("An assert ensures that the manual path is the expected path :-)")
    print("")


if __name__ == "__main__":
    main()
