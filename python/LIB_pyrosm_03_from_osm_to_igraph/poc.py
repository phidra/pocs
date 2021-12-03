#!/usr/bin/env python3

import pyrosm
import pyproj


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


def id_to_osm_way_url(way_id):
    return "https://www.openstreetmap.org/way/{}".format(way_id)


def length_of(geometry):
    # length is returned in meters
    geod = pyproj.Geod(ellps="WGS84")
    return geod.geometry_length(geometry)


def main():
    print("DESCRIPTION :")
    print("    build igraph from PBF file, first using 'driving' network_type")
    print("    message of the POC is to show that ONE SINGLE OSM WAY is splitted into several igraph sub-edges")
    print("    if we choose walking network_type, the number of subedges is DOUBLED (one for each walking direction)")
    print("    BEWARE : we can't (directly) use the OSM data as a routing graph, bc some ways have to be splitted !")
    print("")

    # build igraph from PBF file, first using "driving" network_type :
    INPUT_PBF_FILE = "NOGIT_monaco.osm.pbf"
    osm = pyrosm.OSM(INPUT_PBF_FILE)
    nodes, edges = osm.get_network(nodes=True, network_type="driving")
    g = osm.to_graph(nodes, edges)

    # https://www.openstreetmap.org/way/39839531
    # oneway road, allowed for cars AND pedestrians, with 7 nodes (thus 6 sub-edges)
    oneway_edge_id = 39839531
    EXPECTED_NB_EDGES_WITH_ONEWAY = 6
    print("OSM way = ", id_to_osm_way_url(oneway_edge_id))

    # message of the POC is to show that ONE SINGLE OSM WAY is splitted into several igraph sub-edges :
    igraph_edges = g.es.select(id=oneway_edge_id)
    assert len(igraph_edges) == EXPECTED_NB_EDGES_WITH_ONEWAY
    print("This OSM way has {} sub-edges :".format(len(igraph_edges)))
    for e in igraph_edges:
        d = dict(
            osm_id=e["id"],
            oneway=e["oneway"],
            geometry=e["geometry"],
            length=e["length"],
            u=e["u"],  # u is the OSM node id
            v=e["v"],
            u_seq=e["u_seq"],  # u_seq is the igraph vertex id
            v_seq=e["v_seq"],
        )
        print("\t", d)

    # 'u' represents the source vertex (and v=target) of the sub-edge, with its OSM id :
    print("This OSM way is made of those OSM nodes :")
    for e in igraph_edges:
        print("\t", id_to_osm_node_url(e["u"]))
    last_subedge = igraph_edges[-1]
    print("\t", id_to_osm_node_url(last_subedge["v"]))

    # 'u_seq' represents the source vertex of the sub-edge, with is IGRAPH id :
    print("This OSM way is made of those OSM nodes, retrieved in igraph \\o/ :")
    for e in igraph_edges:
        u_seq = e["u_seq"]
        igraph_vertex = g.vs[u_seq]
        print("\t", id_to_osm_node_url(igraph_vertex["id"]))
    last_vertex = g.vs[last_subedge["v_seq"]]
    print("\t", id_to_osm_node_url(last_vertex["id"]))

    # if we choose "walking" network_type, the number of subedges is DOUBLED (one for each walking direction) :
    nodes_bidirectional, edges_bidirectional = osm.get_network(nodes=True, network_type="walking")
    g_bidirectional = osm.to_graph(nodes_bidirectional, edges_bidirectional)
    igraph_edges_bidirectional = g_bidirectional.es.select(id=oneway_edge_id)
    EXPECTED_BIDIRECTIONAL = 2 * EXPECTED_NB_EDGES_WITH_ONEWAY
    assert len(igraph_edges_bidirectional) == EXPECTED_BIDIRECTIONAL
    print("If we choose 'walking' network_type, the number of subedges is DOUBLED : {}".format(EXPECTED_BIDIRECTIONAL))

    # each subedge has a length (in meters) :
    # to get the FULL geometry of an OSM way (or the full length), we have to concatenate the subedges...
    length_read_from_properties = last_subedge["length"]
    length_computed_from_geometry = length_of(last_subedge["geometry"])
    print("Length of last-subedge in METERS, read in its properties     = ", length_read_from_properties)
    print("Length of last-subedge in METERS, computed from its geometry = ", length_computed_from_geometry)
    are_lengths_equal = abs(length_read_from_properties - length_computed_from_geometry) < 0.1
    print("are both lenghts equal (more or less, with geo precision or rounding) ? ", are_lengths_equal)
    assert are_lengths_equal

    # BEWARE : we can't (directly) use the OSM data as a routing graph, because some ways still have to be splitted !
    # e.g. this way W1 is a single OSM way :
    #       https://www.openstreetmap.org/way/214635117
    # but it is intersected IN ITS MIDDLE by (at least) one other way W2 :
    #       https://www.openstreetmap.org/way/448186736#map=19/43.72566/7.41779
    # thus, to properly build a routing graph from OSM data, we would have to split W1 in two edges
    # (the split would occur where W1 and W2 intersect)


main()
