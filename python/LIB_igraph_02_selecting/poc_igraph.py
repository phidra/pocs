#!/usr/bin/env python3.6

import igraph

print("version = {}".format(igraph.__version__))

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

    g = igraph.Graph()  # by default : undirected

    A = g.add_vertex(name="A", latitude=48.8472743, longitude=2.3385463)
    B = g.add_vertex(name="B", latitude=48.8538388, longitude=2.2667097)
    C = g.add_vertex(name="C", latitude=48.8822442, longitude=2.3355303)
    D = g.add_vertex(name="D", latitude=48.8784585, longitude=2.3682707)
    E = g.add_vertex(name="E", latitude=48.8783875, longitude=2.2781327)
    F = g.add_vertex(name="F", latitude=48.8751455, longitude=2.3496791)
    G = g.add_vertex(name="G", latitude=48.8695761, longitude=2.3778204)
    g.add_vertex(name="H", latitude=48.8404808, longitude=2.2935483)

    # note : instead of using vertex handles, one can use its (special property) name.
    # below, this is illustrated with the 'H' vertex :

    # except for AB, we ignore the returned edge handle of 'add_edge' (as we don't use the edges) :
    AB = g.add_edge(A, B,   weight=5, streetname="Boulevard Saint-Michel")
    g.add_edge(A, C,   weight=10, streetname="Avenue Mozart")
    g.add_edge(A, D,   weight=1, streetname="Place Pigalle")
    g.add_edge(B, C,   weight=8, streetname="Boulevard de la Vilette")
    g.add_edge(B, E,   weight=3, streetname="Avenue de Neuilly")
    g.add_edge(C, F,   weight=5, streetname="Rue de Paradis")
    g.add_edge(D, E,   weight=9, streetname="Boulevard de Belleville")
    g.add_edge(E, G,   weight=5, streetname="Rue Lecourbe")
    g.add_edge(F, "H", weight=2, streetname="Avenue des Champs-Élysées")
    g.add_edge(G, "H", weight=3, streetname="Rue de la Paix")

    # using vertex/edge values :
    print("=== vertex A has coordinates [{} ; {}]".format(A["latitude"], A["longitude"]))
    print("=== vertex E has index {}".format(E.index))
    print("=== vertex with index {} has name '{}'".format(E.index, g.vs[E.index]["name"]))
    print("=== edge AB has streetname '{}'".format(AB["streetname"]))
    print("=== edge AB goes from VERTEX({}) with name '{}' to VERTEX({}) with name '{}'".format(
        AB.source,
        g.vs[AB.source]["name"],
        AB.target,
        g.vs[AB.target]["name"],
    ))

    # filtering with a precise attribute :
    vertices_C = g.vs.select(name="C")
    print("=== The vertices with the name 'C' are :")
    for v in vertices_C:
        print(v["name"])

    # filtering with an attribute range :
    vertices_gt_2_3_longitude = g.vs.select(longitude_gt=2.3)
    print("=== The vertices with a longitude greater that 2.3 are :")
    for v in vertices_gt_2_3_longitude:
        print(v["name"], "longitude={}".format(v["longitude"]))

    # filtering with an attribute in an iterable :
    vertices_consonant = g.vs.select(name_in="BCDFG")
    print("=== The vertices with a consonant name are :")
    for v in vertices_consonant:
        print(v["name"])

    # filtering with a lambda :
    def filtering_function(edge):
        return edge["streetname"].startswith("Rue")

    edges_beginning_with_Rue = g.es.select(filtering_function)
    print("=== The edges with a 'Rue' streetname are :")
    for edge in edges_beginning_with_Rue:
        print(edge["streetname"])

    # NOTE : 'find' instead of 'select' returns first vertex/edge matching the selection :
    first_one = g.es.find(filtering_function)
    print("=== The FIRST edge with a 'Rue' streetname is : ", first_one["streetname"])

    # drawing :
    layout = g.layout_kamada_kawai()
    igraph.plot(g, "/tmp/pouet.png", layout=layout)
    print(g)


my_tests()
