#!/usr/bin/env python3

import igraph

print("version = {}".format(igraph.__version__))


def my_tests():

    g = igraph.Graph()  # by default : undirected

    g.add_vertices(5)  # adding 5 nodes, ranked 0 to 4
    edges = [(0, 1), (1, 2), (3, 2), (3, 0)]
    g.add_edges(edges)

    g.vs["name"] = ["" for _ in range(g.vcount())]  # initialising with empty names
    g.vs["name"] = ["Paris", "Marseille", "Bordeaux", "Lyon", "Liiiiille"]
    g.vs[3]["name"] = "Lille"

    g.es["weight"] = [0 for _ in range(g.ecount())]
    g.es[0]["weight"] = 42
    g.es[1]["weight"] = 19
    g.es[2]["weight"] = 42

    print("Combien de villes reliées à Paris ? ", g.degree()[0])

    # 'select' to find vertices by a property seems powerful :
    node_paris = g.vs.select(name="Paris")
    print("node paris is {}".format(node_paris["name"]))
    edges_42 = g.es.select(weight=42)
    print("edge with weight 42 are {}".format(edges_42["weight"]))

    # drawing :
    igraph.plot(g, "/tmp/pouet.png", layout=g.layout_kamada_kawai())

    # dumping :
    g.write_edgelist("/tmp/graph.edgelist")
    g.write_dot("/tmp/graph.dot")
    g.write_adjacency("/tmp/graph.adjacency")

    print(g)

    # there seems to be no reading from dot yet
    # but graphviz provides gv2gml, and igraph can read gml :
    g2 = igraph.Graph.Read_GML("pouet.gml")
    print("Combien de vertex dans g2 ? {}".format(g2.vcount()))
    print("Nom des 2 premiers vertex de g2 ? {} {}".format(g2.vs[0]["name"], g2.vs[1]["name"]))


my_tests()
