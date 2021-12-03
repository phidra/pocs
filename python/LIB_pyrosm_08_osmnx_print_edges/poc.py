#!/usr/bin/env python3

import pyrosm
import osmnx as ox
from pprint import pprint
import json


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


def id_to_osm_way_url(way_id):
    return "https://www.openstreetmap.org/way/{}".format(way_id)


def to_geojson(points):
    d = {
        "type": "FeatureCollection",
        "features": [
            {
                "type": "Feature",
                "properties": {},
                "geometry": {
                    "type": "LineString",
                    "coordinates": list(points),
                },
            }
        ],
    }
    return json.dumps(d)


def print_edge(u, v, d, G):
    print("===")
    print("=== u = {}   (URL = {})".format(u, id_to_osm_node_url(u)))
    print("=== v = {}   (URL = {})".format(v, id_to_osm_node_url(v)))
    print("=== this edge belongs to way       URL = {}".format(id_to_osm_way_url(d["osmid"])))
    print("=== edge attributes :")
    pprint(d)
    print("=== nodes URL in 'u' attribute :")
    for u_unit in d["u"]:
        print("\t URL u = ", id_to_osm_node_url(u_unit))
    print("=== nodes URL in 'v' attribute :")
    for v_unit in d["v"]:
        print("\t URL v = ", id_to_osm_node_url(v_unit))
    print("=== gemoetry SHAPELY :")
    for coords in d["geometry"].coords:
        print(coords)
    print("=== gemoetry GEOJSON :")
    print(to_geojson(d["geometry"].coords))
    print("=== 'u' attributes :")
    pprint(G.nodes[u])
    print("=== 'v' attributes :")
    pprint(G.nodes[v])


def main():
    print("DESCRIPTION = printing osmnx edges to understand how it works")

    # build from PBF file, first using "driving" network_type :
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

    print("TYPE = ", type(G).__name__)  # MultiDiGraph

    # printing the edges to understand what are their attributes :
    edges_iter = iter(G.edges(data=True))
    u, v, d = next(edges_iter)
    print_edge(u, v, d, G)

    next(edges_iter)
    next(edges_iter)
    next(edges_iter)
    next(edges_iter)
    u, v, d = next(edges_iter)
    print_edge(u, v, d, G)

    for _, _, d in G.edges(data=True):
        # case 1 : if the osmnx edge is a single segment (the simplification didn't modify it), then :
        #   - u (resp. v) are a single int
        #   - the edge doesn't have a 'geometry' attribute
        if "geometry" not in d:
            assert type(d["u"]) == int
            assert type(d["v"]) == int
        if type(d["u"]) == int:
            assert type(d["v"]) == int
            assert "geometry" not in d
        if type(d["v"]) == int:
            assert type(d["u"]) == int
            assert "geometry" not in d

        # case 2 : if the osmnx edge has a complex geometry (it comes from the merge of several unitary edges), then :
        #   - u (resp. v) are a list
        #   - the edge has a 'geometry' attribute
        # in this case, the geometry has N+1 points, where N is the number of unitary edges
        if "geometry" in d:
            assert type(d["u"]) == list
            assert type(d["v"]) == list
        if type(d["u"]) == list:
            assert type(d["v"]) == list
            assert "geometry" in d
        if type(d["v"]) == list:
            assert type(d["u"]) == list
            assert "geometry" in d

    print("All is OK")


main()
