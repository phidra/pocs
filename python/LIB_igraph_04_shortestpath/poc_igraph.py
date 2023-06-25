#!/usr/bin/env python3

import igraph


def my_tests():

    g = igraph.Graph()  # by default : undirected

    #           10
    #     ---------------
    #    /               \
    #   /   5         8   \
    #  A---------B---------C
    #  |         |         |
    #  |1        |3        |5
    #  |         |         |
    #  D---------E         F
    #       9    |         |
    #            |5        |2
    #            |         |
    #            G---------H
    #                 3

    A = g.add_vertex(name="A")
    B = g.add_vertex(name="B")
    C = g.add_vertex(name="C")
    D = g.add_vertex(name="D")
    E = g.add_vertex(name="E")
    F = g.add_vertex(name="F")
    G = g.add_vertex(name="G")
    H = g.add_vertex(name="H")

    g.add_edge(A, B, weight=5, streetname="Boulevard Saint-Michel")
    g.add_edge(A, C, weight=10, streetname="Avenue Mozart")
    g.add_edge(A, D, weight=1, streetname="Place Pigalle")
    g.add_edge(B, C, weight=8, streetname="Boulevard de la Vilette")
    g.add_edge(B, E, weight=3, streetname="Avenue de Neuilly")
    g.add_edge(C, F, weight=5, streetname="Rue de Paradis")
    g.add_edge(D, E, weight=9, streetname="Boulevard de Belleville")
    g.add_edge(E, G, weight=5, streetname="Rue Lecourbe")
    g.add_edge(F, H, weight=2, streetname="Avenue des Champs-Élysées")
    g.add_edge(G, H, weight=3, streetname="Rue de la Paix")

    SOURCE = A
    TARGETS = [H]  # the function expects a LIST of targets (here, containing a single element)
    WEIGHTS = g.es["weight"]
    print("computing shortest path from {} to {}".format(SOURCE["name"], TARGETS[0]["name"]))

    shortest_paths = g.get_shortest_paths(SOURCE, TARGETS, WEIGHTS)

    # a shortest path is returned for each target, stored as an index-list :
    shortest_path = shortest_paths[0]
    assert(shortest_path == [0, 1, 4, 6, 7])

    # converting index-list into name-list :
    name_list = g.vs[shortest_path]["name"]
    assert(name_list == ["A", "B", "E", "G", "H"])
    path_as_string = "".join(name_list)
    assert(path_as_string == "ABEGH")
    print("shortest path is : {}".format(path_as_string))


my_tests()
