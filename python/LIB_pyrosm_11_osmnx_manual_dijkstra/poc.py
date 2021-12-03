#!/usr/bin/env python3

import sys
import pyrosm
import osmnx as ox
import networkx as nx
from heapq import heappush, heappop
from pprint import pprint


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


def id_to_osm_way_url(way_id):
    return "https://www.openstreetmap.org/way/{}".format(way_id)


def compute_path(source, target, parents):
    path = []  # initially, reversed

    parent = target
    while parent != source:
        path.append(parent)
        parent = parents[parent]
    path.append(source)
    path.reverse()
    return path


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

    while queue:
        cost, settled_node = heappop(queue)
        settled_nodes.add(settled_node)
        relax(G, settled_node)
        if settled_node == target:
            break

    path = compute_path(source, target, parents)
    return path


def main():
    print("DESCRIPTION = manual dijkstra")

    # STEP 1 = building + filtering graph :
    INPUT_PBF_FILE = "NOGIT_monaco.osm.pbf"
    minx = 7.417826056480407
    maxx = 7.420363426208495
    miny = 43.72635527380567
    maxy = 43.72775470745523

    osm = pyrosm.OSM(INPUT_PBF_FILE, bounding_box=[minx, miny, maxx, maxy])
    nodes, edges = osm.get_network(nodes=True, network_type="walking")
    G_nonsimplified = osm.to_graph(nodes, edges, graph_type="networkx")

    # osmnx can't work with the "geometry" property, apparently...
    # cf. https://stackoverflow.com/questions/65568844/typeerror-unhashable-type-linestring-when-using-ox-simplify-graph/65592832#65592832  # noqa
    for _, _, d in G_nonsimplified.edges(data=True):
        d.pop("geometry", None)

    G = ox.simplify_graph(G_nonsimplified)

    # SOURCE / TARGET NODES :
    # https://www.openstreetmap.org/node/263077002
    source_vertex_osm_id = 263077002
    # https://www.openstreetmap.org/node/7781960118
    target_vertex_osm_id = 7781960118
    print("SOURCE VERTEX IS : ", id_to_osm_node_url(source_vertex_osm_id))
    print("TARGET VERTEX IS : ", id_to_osm_node_url(target_vertex_osm_id))

    # the shortest path between this source+target is expected to go through these nodes :
    #       https://www.openstreetmap.org/node/263077002
    #       https://www.openstreetmap.org/node/263077003
    #       https://www.openstreetmap.org/node/5046302791
    #       https://www.openstreetmap.org/node/5046302792
    #       https://www.openstreetmap.org/node/7781960118

    expected_shortest_path = [
        263077002,
        263077003,
        5046302791,
        5046302792,
        7781960118,
    ]

    # the shortest path between this source+target is expected to go through these edges :
    #       https://www.openstreetmap.org/way/849737733
    #       https://www.openstreetmap.org/way/849737754
    #       https://www.openstreetmap.org/way/517029021
    #       https://www.openstreetmap.org/way/849737736

    for node, node_data in G.nodes(data=True):
        print("")
        print("node = ", node)
        print("node_data : ")
        pprint(node_data)

    for u, v, edge_data in G.edges(data=True):
        print("")
        print("edge = ", u, " -> ", v)
        print("edge_data : ")
        pprint(edge_data)

    print("")
    print("source_node is : ")
    pprint(G.nodes[source_vertex_osm_id])

    print("")
    print("target_node is : ")
    pprint(G.nodes[target_vertex_osm_id])

    path = nx.shortest_path(G, source=source_vertex_osm_id, target=target_vertex_osm_id, weight="length")
    print("NETWORKX shortest path goes through these nodes :")
    for node in path:
        print("\t", node, " = ", id_to_osm_node_url(node))
    assert path == expected_shortest_path
    print("A previous assert ensures that it is the expected path :-)")
    print("")

    path2 = manual_dijkstra(G, source=source_vertex_osm_id, target=target_vertex_osm_id, weight="length")
    print("MANUAL shortest path goes through these nodes :")
    for node in path2:
        print("\t", node, " = ", id_to_osm_node_url(node))
    assert path2 == expected_shortest_path
    print("A previous assert ensures that it is the expected path :-)")
    print("")


main()
