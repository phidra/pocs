#!/usr/bin/env python3.6

import igraph


def my_tests():
    g = igraph.Graph()  # by default : undirected

    g.add_vertex(name="Paris", latitude=48.8534, longitude=2.3488)
    g.add_vertex(name="Marseille", latitude=43.296398, longitude=5.370000)
    g.add_vertex(name="Nice", latitude=43.675819, longitude=7.289429)
    g.add_vertex(name="Lyon", latitude=45.764043, longitude=4.835659)
    g.add_vertex(name="Bordeaux", latitude=44.836151, longitude=-0.580816)
    g.add_vertex(name="Lille", latitude=50.633333, longitude=3.066667)
    g.add_vertex(name="Rennes", latitude=48.114700, longitude=-1.679400)
    g.add_vertex(name="Brest", latitude=48.390394, longitude=-4.486076)
    g.add_vertex(name="Strasbourg", latitude=48.5734053, longitude=7.7521113)
    g.add_vertex(name="Antony", latitude=48.753899, longitude=2.297500)
    g.add_vertex(name="Pau", latitude=43.299999, longitude=-0.370000)
    g.add_vertex(name="Collioures", latitude=42.526752, longitude=3.084631)

    # vertex names on the layout :
    g.vs["label"] = g.vs["name"]

    # using the vertices coordinates as coordinates on the layout :
    # NOTE : a projection would be cleaner, but this is still a good first step
    layout = igraph.layout.Layout()
    for v in g.vs:
        layout.append((v["longitude"], -v["latitude"]))

    # plotting :
    igraph.plot(g, "/tmp/geolayout.png", layout=layout, margin=100, vertex_label_dist=1)


my_tests()
