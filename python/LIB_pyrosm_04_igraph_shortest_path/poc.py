#!/usr/bin/env python3

import pyrosm


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


def id_to_osm_way_url(way_id):
    return "https://www.openstreetmap.org/way/{}".format(way_id)


def main():
    print("DESCRIPTION = compute shortest path on OSM data")

    # build igraph from PBF file, first using "driving" network_type :
    INPUT_PBF_FILE = "NOGIT_monaco.osm.pbf"
    osm = pyrosm.OSM(INPUT_PBF_FILE)
    nodes, edges = osm.get_network(nodes=True, network_type="walking")
    g = osm.to_graph(nodes, edges)

    # retrieve igraph source / target:
    # https://www.openstreetmap.org/node/4452216546
    source_vertex_osm_id = 4452216546
    source_vertex = g.vs.find(id=source_vertex_osm_id)
    print("SOURCE VERTEX IS : ", id_to_osm_node_url(source_vertex_osm_id))

    # https://www.openstreetmap.org/node/7781127878
    target_vertex_osm_id = 7781127878
    target_vertex = g.vs.find(id=target_vertex_osm_id)
    print("TARGET VERTEX IS : ", id_to_osm_node_url(target_vertex_osm_id))

    # the shortest path between this source+target is expected to go through these edges :
    expected_shortest_path = [
        448186736,  # https://www.openstreetmap.org/way/448186736
        214635117,  # https://www.openstreetmap.org/way/214635117
        177049744,  # https://www.openstreetmap.org/way/177049744
        158215203,  # https://www.openstreetmap.org/way/158215203
    ]

    igraph_shortest_paths = g.get_shortest_paths(source_vertex, [target_vertex], g.es["length"])

    # a shortest path is returned for each target, stored as an index-list :
    path = igraph_shortest_paths[0]
    print("OSM nodes along the path = ", path)
    for vertex in path:
        print("\t", id_to_osm_node_url(g.vs[vertex]["id"]))

    # the shortest path is a sequence of (igraph) nodes
    # to get the OSM edges back, we first must get edges of nodepairs.
    # get_eid has randomness, but is be good enough for this POC :
    #       https://igraph.org/python/doc/api/igraph._igraph.GraphBase.html#get_eid
    # to properly retrieve edges, we could also have used this :
    #       https://igraph.org/python/doc/api/igraph._igraph.GraphBase.html#incident

    path_osm = []
    for vertex_from, vertex_to in zip(path[1:], path[:-1]):
        igraph_edge = g.get_eid(vertex_from, vertex_to)
        osm_way_id = g.es[igraph_edge]["id"]
        # as pyrosm splits an OSM way into several igraph edges,
        # we only store an osm way if it's not already in the sequence of ways
        if not path_osm or path_osm[-1] != osm_way_id:
            path_osm.append(osm_way_id)

    # here, we have the shortest path, as osm ways :
    print("The shortest path as OSM ways is :")
    for osm_way in path_osm:
        print("\t", id_to_osm_way_url(osm_way))
    is_shortest_path_equal_to_expected = path_osm == expected_shortest_path
    print("Is the shortest path the expected one ?", is_shortest_path_equal_to_expected)
    assert is_shortest_path_equal_to_expected


main()
