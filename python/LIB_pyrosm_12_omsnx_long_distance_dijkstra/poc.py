#!/usr/bin/env python3

import sys
import pyrosm
import osmnx as ox
import networkx as nx
from heapq import heappush, heappop
import matplotlib.pyplot as plt


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


def manual_dijkstra(G, source, target, weight):
    costs = {}
    for node in G.nodes():
        costs[node] = sys.maxsize  # initial costs are huge
    costs[source] = 0  # source cost is null

    settled_nodes = set()
    parents = {source: source}
    queue = []
    heappush(queue, (0, source))

    def relax(G, relaxed):
        relaxed_cost = costs[relaxed]
        for successor, multiedge_properties in G[relaxed].items():
            if successor in settled_nodes:
                continue
            edge_property = multiedge_properties[0]  # for the sake of simplicity, we ignore parallel edges
            successor_old_cost = costs[successor]
            successor_new_cost = relaxed_cost + edge_property[weight]

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

    while queue:
        cost, settled_node = heappop(queue)
        settled_nodes.add(settled_node)
        relax(G, settled_node)
        if settled_node == target:
            break

    path = compute_path(source, target, parents)
    return path


def main():
    print("DESCRIPTION = manual dijkstra on a bigger scale (whole monaco)")

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

    fig, ax = plt.subplots(figsize=(12.80, 10.24))
    size_in_pixels = fig.get_size_inches() * fig.dpi
    print("SIZE INCHES = ", fig.get_size_inches())
    print("SIZE DPI = ", fig.dpi)
    print("FIGSIZE = ", size_in_pixels)
    print("type(FIGSIZE) = ", type(size_in_pixels).__name__)  # ndarray
    ox.plot_graph(G, ax=ax, bgcolor="k", node_color="r", edge_color="k", edge_linewidth=0.8, show=False)

    dumped = "/tmp/graph12.png"
    fig.savefig(dumped)
    print("DUMPED GRAPH IN : ", dumped)

    # SOURCE :
    # https://www.openstreetmap.org/node/263077002
    source_vertex_osm_id = 263077002
    print("SOURCE VERTEX IS : ", id_to_osm_node_url(source_vertex_osm_id))

    # TARGET :
    # https://www.openstreetmap.org/node/1868736741
    target_vertex_osm_id = 1868736741
    print("TARGET VERTEX IS : ", id_to_osm_node_url(target_vertex_osm_id))

    # the "reference" shortest path is the networkx one :
    # (for the record, it is stored in the "expected_shortest_path.json" file)
    reference_path = nx.shortest_path(G, source=source_vertex_osm_id, target=target_vertex_osm_id, weight="length")
    print("NETWORKX shortest path goes through these nodes (first ones, at least) :")
    for idx, node in enumerate(reference_path):
        print("\t", node, " = ", id_to_osm_node_url(node))
        if idx > 8:
            break
    print("\t...")
    print("")

    manual_path = manual_dijkstra(G, source=source_vertex_osm_id, target=target_vertex_osm_id, weight="length")
    print("MANUAL shortest path goes through these nodes (first ones, at least) :")
    for idx, node in enumerate(manual_path):
        print("\t", node, " = ", id_to_osm_node_url(node))
        if idx > 8:
            break
    print("\t...")
    assert manual_path == reference_path
    print("An assert ensures that the manual path is the expected path :-)")
    print("")


main()
