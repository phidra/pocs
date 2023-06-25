#!/usr/bin/env python3.6

import networkx as nx

print("version = {}".format(nx.__version__))

description = r"""
           10
     ---------------
    /               \
   /   5         8   \
  A---------B---------C
  |         |         |
  |1        |3        |5
  |         |         |
  D---------E         F
       9    |         |
            |5        |2
            |         |
            G---------H
                 3
"""


def my_tests():

    g = nx.Graph()  # by default : undirected

    # difference igraph/nx : add_node and add_vertex don't return
    g.add_node("A", latitude=48.8472743, longitude=2.3385463)  #
    g.add_node("B", latitude=48.8538388, longitude=2.2667097)
    g.add_node("C", latitude=48.8822442, longitude=2.3355303)
    g.add_node("D", latitude=48.8784585, longitude=2.3682707)
    g.add_node("E", latitude=48.8783875, longitude=2.2781327)
    g.add_node("F", latitude=48.8751455, longitude=2.3496791)
    g.add_node("G", latitude=48.8695761, longitude=2.3778204)
    g.add_node("H", latitude=48.8404808, longitude=2.2935483)

    A = g.nodes["A"]
    E = g.nodes["E"]
    print("A = ", A)
    print("E = ", E)

    # except for AB, we ignore the returned edge handle of 'add_edge' (as we don't use the edges) :
    g.add_edge("A", "B", weight=5, streetname="Boulevard Saint-Michel")
    g.add_edge("A", "C", weight=10, streetname="Avenue Mozart")
    g.add_edge("A", "D", weight=1, streetname="Place Pigalle")
    g.add_edge("B", "C", weight=8, streetname="Boulevard de la Vilette")
    g.add_edge("B", "E", weight=3, streetname="Avenue de Neuilly")
    g.add_edge("C", "F", weight=5, streetname="Rue de Paradis")
    g.add_edge("D", "E", weight=9, streetname="Boulevard de Belleville")
    g.add_edge("E", "G", weight=5, streetname="Rue Lecourbe")
    g.add_edge("F", "H", weight=2, streetname="Avenue des Champs-Élysées")
    g.add_edge("G", "H", weight=3, streetname="Rue de la Paix")

    AB = g.edges["A", "B"]
    print("AB = ", AB)
    print("weight of AB (1) = ", g["A"]["B"]["weight"])
    print("weight of AB (2) = ", g.get_edge_data("A", "B")["weight"])
    print("")

    # using vertex/edge values :
    print("=== vertex A has coordinates [{} ; {}]".format(A["latitude"], A["longitude"]))
    print("=== vertex E has neighbors {}".format(g.adj["E"]))
    print("=== edge AB has streetname '{}'".format(AB["streetname"]))
    print("")

    # ITERATION OVER ALL EDGES :
    # edges of 'B' (as the graph is undirected, this will iterate over all edges linked to B) :
    B_edges = g.edges("B")
    print("Edges of B :")
    print(B_edges)
    for edge in B_edges:
        print("\t EDGE = ", edge)
        # for networkx edges are just tuples with two node instances:
        print("\t type = ", type(edge).__name__)
        source, target = edge[0], edge[1]
        print("\t This edge goes from '{}' to '{}'".format(source, target))
        print("")
    print("Edges of A :", g.edges("A"))

    # on a directed graph, iteration on edges only sees out-edges :
    g2 = nx.DiGraph()  # directed
    g2.add_node("A")  #
    g2.add_node("B")
    g2.add_node("C")
    g2.add_node("D")
    g2.add_edge("A", "B")
    g2.add_edge("B", "C")
    g2.add_edge("B", "D")

    print("Edges on a directed graph : ", g2.edges("B"))  # will only see BC and BD (not AB)

    # this is a main difference between igraph and networkx : no "find" or "select", we must walk the graph  :

    # nodes/edges with a fixed attribute :
    edges_with_weight_3 = [(u, v) for u, v, weight in g.edges(data="weight") if weight == 3]
    print("edges with weight 3 :")
    for u, v in edges_with_weight_3:
        print("\t ", u, "->", v)

    nodes_with_precise_latitude = [n for n, n_attrs in g.nodes(data=True) if n_attrs["latitude"] == 48.8538388]
    print("nodes with latitude 48.8538388 :")
    for n in nodes_with_precise_latitude:
        print("\t ", n)

    # see :
    # https://networkx.org/documentation/stable/reference/classes/generated/networkx.Graph.nodes.html#networkx.Graph.nodes
    # https://networkx.org/documentation/stable/reference/classes/generated/networkx.Graph.edges.html#networkx.Graph.edges


my_tests()
