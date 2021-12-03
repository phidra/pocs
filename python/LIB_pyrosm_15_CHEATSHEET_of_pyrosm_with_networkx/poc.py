#!/usr/bin/env python3

import pyrosm
import osmnx as ox
from pprint import pprint


DESCRIPTION = """
DESCRIPTION = quick reminder on how to use a networkx graph that was loaded with pyrosm/osmnx :

WHO DOES WHAT :
    - pyrosm loads a graph from an OSM file
    - osmnx "simplifies" it (by merging consecutive elementary edges if need be)

WARNING = the resulting graph is a multigraph (each nodepair (u,v) might have SEVERAL edges, but in reality it's fairly rare)

TL; DR = syntax is pretty confusing :
    - G.nodes()           iterate over ALL graph nodes (and their properties if need be)
    - G.nodes[n]          get properties of ONE NODE of the graph
    - G[n]                iterate over the NEIGHBOURS of a node (and the properties of the linking edge if need be)
    - G[u][v]             get properties of ONE EDGE (alternative=G.get_edge_data(...))
    - G.edges(data=True)  iterate over ALL graph edges (and their properties if need be)

NODE PROPERTIES = for each node, pyrosm/osmnx define the following properties :
    - changeset=0,
    - geometry=<shapely.geometry.point.Point object at 0x7f703e3630d0>,
    - osmid=7265762803,
    - tags=None,
    - timestamp=1583398246,
    - version=1,
    - x=7.4256802,
    - y=43.7393746

Notably, the ID of a node is it's OSM id.

EDGE PROPERTIES = for each edge, pyrosm/osmnx define the following properties (probably tag-dependent) :
    - access=None
    - bicycle=None
    - bridge=None
    - foot=None
    - footway=None
    - highway=footway
    - junction=None
    - lanes=None
    - lit=None
    - maxspeed=None
    - motor_vehicle=None
    - name=None
    - oneway=None
    - overtaking=None
    - psv=None
    - ref=None
    - service=None
    - sidewalk=None
    - smoothness=None
    - surface=None
    - tunnel=None
    - osmid=778494670
    - timestamp=1583398246
    - version=1
    - tags=None
    - osm_type=way
    - u=[7265762800, 7265762801, 7265762802, 7265762803]
    - v=[7265762800, 7265762801, 7265762802, 7265762799]
    - length=32.598
    - geometry=LINESTRING (7.4256802 43.7393746, 7.4257418 43.739354, 7.4257853 43.7394266, 7.4259011 43.7394524, 7.4260082 43.7394488)

Notably, the property 'osmid' of an edge contains the OSM id of the way that contains the edge.

WARNING : a single way OSM might be splitted in SEVERAL graph edges (there is no 1 to 1 matching).

Getting the osmids of the nodes in an edge geometry seem to be tricky :
    - pyrosm gets elementary edges, and stores the nodes (https://pyrosm.readthedocs.io/en/latest/basics.html#read-nodes-and-edges-first-step)
    - after osmnx's simplification props 'u' and 'v' seem to contains the nodes used by an edge...
    - ... but it seems that their order is lost
"""  # noqa


def id_to_osm_node_url(vertex_id):
    return "https://www.openstreetmap.org/node/{}".format(vertex_id)


def main():
    print(DESCRIPTION)

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

    # On itère sur les nodes du graphe pyrosm avec G.nodes() :
    print("")
    print("====== ESSAI N°1 = comment itérer sur les nodes + on récupère quoi ?")
    counter = 0
    for node_id in G.nodes():
        counter += 1
        if counter > 3:
            break

        # les ids de nodes ainsi récupérés sont les ids OSM :
        print("ID = ", node_id)
        print("TYPE = ", type(node_id).__name__)
        print("URL = ", id_to_osm_node_url(node_id))
        print()

    # On itère sur les nodes ET LEURS PROPRIÉTÉS avec G.nodes(data=True):
    print("")
    print("====== ESSAI N°2 : comment itérer sur les nodes et leurs propriétés ?")
    counter = 0
    for node_id, node_props in G.nodes(data=True):
        counter += 1
        if counter > 3:
            break

        # les propriétés des nodes dans le graphe pyrosm sont des dict.
        # pyrosm définit les propriétés suivantes (les valeurs ne sont données que pour l'exemple) :
        #    - 'changeset': 0,
        #    - 'geometry': <shapely.geometry.point.Point object at 0x7f703e3630d0>,
        #    - 'osmid': 7265762803,
        #    - 'tags': None,
        #    - 'timestamp': 1583398246,
        #    - 'version': 1,
        #    - 'x': 7.4256802,
        #    - 'y': 43.7393746
        print("ID = ", node_id)
        print("TYPE = ", type(node_id).__name__)
        print("URL = ", id_to_osm_node_url(node_id))
        print("PROPS TYPE = ", type(node_props).__name__)
        print("PROPS = ")
        pprint(node_props)
        print()

    # On récupère les propriétés d'un node donné avec G.nodes[id] :
    print("")
    print("====== ESSAI N°3 : comment récupérer les propriétés d'un node donné ?")
    a_random_node_id = 7265762803
    print("Les propriétés du node {} sont :".format(a_random_node_id))
    props = G.nodes[a_random_node_id]
    print("PROPS TYPE = ", type(props).__name__)
    print("PROPS = ")
    pprint(props)
    print()

    # On itère sur les noeuds voisins d'un node avec G[node_id] :
    print("")
    print("====== ESSAI N°4 : comment itérer sur les noeuds voisins d'un node (et leurs propriétés) :")
    print("Quels sont les noeuds voisins de {} ?".format(a_random_node_id))
    for neighbour in G[a_random_node_id]:
        neighbour_props = G.nodes[neighbour]
        print("\t - {} ({} ; {})".format(neighbour, neighbour_props["x"], neighbour_props["y"]))

    # On itère sur les edges voisins d'un node avec G[node_id][edge_head] :
    # (incidemment, ça indique également comment accéder aux propriétés d'un edge (u,v) donné)
    print("")
    print("====== ESSAI N°5 : comment itérer sur les edges voisins d'un node (et leurs propriétés) :")
    print("Quels sont les edges voisins de {} ?".format(a_random_node_id))
    for neighbour_node, multi_edges_props in G[a_random_node_id].items():
        # multi_edges_props = G[a_random_node_id][neighbour_node]
        # syntaxe équivalente (ou presque : elle permet de fallback sur une default-value plutôt que d'exceptionner) :
        # multi_edges_props = G.get_edge_data(a_random_node_id, neighbour_node)
        print("\tLes edge vers {} (il y en a {})".format(neighbour_node, len(multi_edges_props)))
        for edge_number, edge_props in multi_edges_props.items():
            print("\t->edge #{}:".format(edge_number))
            for key, value in edge_props.items():
                print("\t  {}={}".format(key, value))

    # On itère sur tous les edges du graphe avec G.edges() :
    print("")
    print("====== ESSAI N°6 : comment itérer sur tous les edges du graphe (et leurs propriétés) :")
    counter = 0
    for edge_tail, edge_head, edge_props in G.edges(data=True):
        counter += 1
        if counter > 3:
            break
        print("EDGE from {} to {} :".format(edge_tail, edge_head))
        for key, value in edge_props.items():
            print("\t  {}={}".format(key, value))


main()
