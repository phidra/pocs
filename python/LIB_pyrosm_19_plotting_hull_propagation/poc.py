#!/usr/bin/env python3

from pathlib import Path
import sys
from contextlib import contextmanager
from heapq import heappush, heappop
from typing import Tuple, List

import pyrosm
import osmnx as ox
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.axes import Axes
from matplotlib.figure import Figure
from shapely.geometry import MultiPoint


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


@contextmanager
def keep_axes_limits(ax):
    xlim_before_second_drawing = ax.get_xlim()
    ylim_before_second_drawing = ax.get_ylim()
    try:
        yield ax
    finally:
        ax.set_xlim(xlim_before_second_drawing)
        ax.set_ylim(ylim_before_second_drawing)


def compute_settled_nodes_hull(G, settled_nodes) -> Tuple[List[float], List[float]]:
    multipoint = []
    for node in settled_nodes:
        multipoint.append((G.nodes[node]["x"], G.nodes[node]["y"]))
    hull = MultiPoint(multipoint).convex_hull
    hull_lngs = [p[0] for p in hull.exterior.coords]
    hull_lats = [p[1] for p in hull.exterior.coords]
    return hull_lngs, hull_lats


def draw_subgraph(output_dir, fig, ax, G, nodes, edges, file_counter, hull):
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
            bgcolor="k",
            node_size=0,
            edge_color="r",
            edge_linewidth=3,
            show=False,
        )

    hull_ax = draw_hull(fig, ax, hull)

    dumped = output_dir / "graph14-{:04d}.png".format(file_counter)
    fig.savefig(dumped)
    print("ADDING EDGE AND DUMPING IN : ", dumped)

    hull_ax.remove()

    return dumped


def plot_nodes(output_dir, fig, ax, G, parents, nodes, file_counter, hull):
    """
    Given a bunch of nodes, plot their parent edges on an existing axe.
    A node's parent edge is the last edge on the shortest path from source to node.
    """

    # building a minimalistic graph with all the nodes and their parents :
    nodes_to_draw = set([])
    edges_to_draw = []
    for node in nodes:
        u = node  # the settled node
        v = parents[u]  # its parent
        if u == v:
            # guard (notably to handle source node)
            continue
        nodes_to_draw.add(u)
        nodes_to_draw.add(v)
        edges_to_draw.append((u, v))

    draw_subgraph(output_dir, fig, ax, G, nodes_to_draw, edges_to_draw, file_counter, hull)

    return file_counter + 1


def manual_dijkstra(output_dir, fig, ax, G, source, target, weight):
    costs = {}
    for node in G.nodes():
        costs[node] = sys.maxsize  # initial costs are huge
    costs[source] = 0  # source cost is null

    settled_nodes = set()
    parents = {source: source}
    queue = []
    heappush(queue, (0, source))

    file_counter = 1

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
            hull = compute_settled_nodes_hull(G, settled_nodes)
            file_counter = plot_nodes(output_dir, fig, ax, G, parents, nodes_to_plot, file_counter, hull)
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


def draw_hull(fig, ax, hull):
    """ configure axes for hull drawing + draw hull """
    hull_ax = fig.add_axes(ax.get_position(), sharex=ax, sharey=ax)
    hull_ax.patch.set_alpha(0)  # background of hull-axes must be transparent

    # inspired by osmnx.plot.py:_config_ax :
    hull_ax.margins(0)
    for s in hull_ax.spines.values():
        s.set_visible(False)
    hull_ax.get_xaxis().set_visible(False)
    hull_ax.get_yaxis().set_visible(False)

    # draw hull :
    hull_lngs, hull_lats = hull
    hull_ax.fill(hull_lngs, hull_lats, color=(0, 0, 1, 0.3))
    return hull_ax


def step2_draw_base_graph(output_dir: Path, G: nx.MultiDiGraph) -> Tuple[Figure, Axes]:
    fig, ax = plt.subplots(figsize=(12.80, 10.24))
    fig.tight_layout(pad=0, h_pad=0, w_pad=0)
    size_in_pixels = fig.get_size_inches() * fig.dpi
    print("SIZE INCHES = ", fig.get_size_inches())
    print("SIZE DPI = ", fig.dpi)
    print("FIGSIZE = ", size_in_pixels)
    print("type(FIGSIZE) = ", type(size_in_pixels).__name__)  # ndarray
    ox.plot_graph(G, ax=ax, bgcolor="k", node_size=0, edge_color="k", edge_linewidth=0.5, show=False)
    return fig, ax


def main():

    output_dir = Path(sys.argv[1])
    print("Using output_dir = {}".format(output_dir))
    assert(output_dir.is_dir())

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
    assert(path == reference_path)
    print("An assert ensures that the manual path is the expected path :-)")
    print("")


if __name__ == "__main__":
    main()
