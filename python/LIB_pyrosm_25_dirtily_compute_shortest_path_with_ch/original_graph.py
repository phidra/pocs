#!/usr/bin/env python3

from pathlib import Path
from dataclasses import dataclass
from typing import List, Optional, Tuple
import json

import pyrosm
import osmnx
import networkx as nx
from shapely.geometry import Polygon, shape


@dataclass
class OriginalGraph:
    """
    Structure representing an original graph (typically, built from an OSM extract + a filtering polygon).
    The prefix "Original" is there to oppose this (input) graph to the "contracted" one.

    Here is the data pipeline :
                                             (pyrosm)
        OSM extract (+ optional polygon) -------------->  networkx graph (non-simplified)

                                              (osmnx)
        networkx graph (non-simplified)  -------------->  networkx graph (simplified)

                                         (_order_nodes)
        networkx graph (simplified)      --------------> networkx graph (with 'order' attribute) + NodeOrders
    """

    # subtypes :
    NodeOrders = List[int]  # index=order  ;  value=osmid

    @dataclass
    class DumpedFiles:
        """
        Collection of files representing a graph.
        These files are designed to be usable by RoutingKit.
        """
        # in these file, one line per edge :
        tailsfile: Path
        headsfile: Path
        weightsfile: Path

        # in this file, one line per node :
        osmidsfile: Path

    # members :
    graph: nx.MultiDiGraph
    orders: NodeOrders

    # constants :
    NO_POLYGON = "NONE"  # sentinel string to indicate that we don't want to use a polygon

    # from a given OSM extract, pyrosm can build several types of networks :
    # (not an enum to allow other values, cf get_network doc : https://pyrosm.readthedocs.io/en/latest/reference.html)
    NETWORK_WALKING = "walking"
    NETWORK_CYCLING = "cycling"
    NETWORK_DRIVING = "driving"

    @staticmethod
    def create_from_osm_extract(
            osm_extract: Path,
            polygon: Optional[Polygon],
            network_type: str = NETWORK_WALKING,
    ) -> "OriginalGraph":

        # STEP 1 = use pyrosm to read OSM file and convert it to networkx graph :
        osm = pyrosm.OSM(str(osm_extract), bounding_box=polygon)
        nodes, edges = osm.get_network(nodes=True, network_type="walking")
        nonsimplified_nxgraph = osm.to_graph(nodes, edges, graph_type="networkx")

        # STEP 2 = osmnx can't work with the "geometry" property, apparently...
        # cf. https://stackoverflow.com/questions/65568844/typeerror-unhashable-type-linestring-when-using-ox-simplify-graph/65592832#65592832  # noqa
        # we remove it (don't worry : the edges/nodes geometry will still be in the graph !)
        for _, _, d in nonsimplified_nxgraph.edges(data=True):
            d.pop("geometry", None)

        # STEP 3 = use osmnx to simplify the graph :
        nxgraph = osmnx.simplify_graph(nonsimplified_nxgraph)

        # STEP 4 = for each node, defines an integer id (its "order") to handle it :
        orders = OriginalGraph._order_nodes(nxgraph)

        return OriginalGraph(graph=nxgraph, orders=orders)

    @staticmethod
    def _order_nodes(nxgraph: nx.MultiDiGraph) -> NodeOrders:
        """
        Attribute an integer handle "order" to each node.

        For a given node, conversion between order and osmid is possible in both directions :
            - the graph is mutated to add a "order" attribute to each node
            - an array of osmids "orders" is computed, where orders[node_order] == node_osmid

        Returns "orders", the array of osmids.
        """

        current_order = 0
        orders = []
        for node_osmid, node_props in nxgraph.nodes(data=True):
            node_props["order"] = current_order
            orders.append(node_osmid)
            current_order += 1

        assert nxgraph.number_of_nodes() == len(orders)
        return orders

    @staticmethod
    def polygon_from_file(polygon_file: Path) -> Optional[Polygon]:
        """
        Parse a geojson polygon, and returns the corresponding shapely Polygon.
        """
        if not polygon_file or polygon_file == OriginalGraph.NO_POLYGON:
            return None

        # ignoring all but first geometry of the file :
        with open(polygon_file, "r") as f:
            data = json.load(f)
            feature = data["features"][0]
            polygon = shape(feature["geometry"])

        return polygon

    def dump_files(self, base_dir: Optional[Path]) -> "OriginalGraph.DumpedFiles":
        """
        Dumps the graph on the disk (typically in order that RoutingKit uses it) in several files.
        Edges are dumped in multiple files (e.g. a file from the tail node, another file from the head node, etc.)
        Node's orders is dumped in an other additional file.
        """

        tails, heads, weights = self._get_edge_as_arrays()

        if base_dir is None:
            base_dir = Path.cwd()

        print("")
        print("Dumping files :")

        # the length of each of those files is the number of edges :
        headsfile = base_dir / "NOGIT_heads.txt"
        tailsfile = base_dir / "NOGIT_tails.txt"
        weightsfile = base_dir / "NOGIT_weights.txt"

        print("Dumping {} tails to : {}".format(len(tails), tailsfile))
        self._dump_array(tailsfile, tails)

        print("Dumping {} heads to : {}".format(len(heads), headsfile))
        self._dump_array(headsfile, heads)

        print("Dumping {} weights to : {}".format(len(weights), weightsfile))
        self._dump_array(weightsfile, weights)

        # the length of this file is the number of nodes :
        osmidsfile = base_dir / "NOGIT_osmids.txt"
        print("Dumping {} osmids to : {}".format(len(self.orders), osmidsfile))
        self._dump_array(osmidsfile, self.orders)

        return self.DumpedFiles(tailsfile, headsfile, weightsfile, osmidsfile)

    def _get_edge_as_arrays(self) -> Tuple[List[int], List[int], List[int]]:
        """ Converts the graph's edges into arrays of properties (e.g. an array with the edge's tail nodes, etc.)"""

        # dumping edges in a format usable by RoutingKit :
        tails = []
        heads = []
        weights = []
        for edge_tail, edge_head, edge_props in self.graph.edges(data=True):
            # the graph is a MULTIgraph : each nodepair (head, tail) might have SEVERAL edges.
            # it is irrelevant here though, bc when we iterate over G.edges , one edge at a time seem to be returned
            tails.append(self.graph.nodes[edge_tail]["order"])
            heads.append(self.graph.nodes[edge_head]["order"])
            weights.append(edge_props["length"])

        assert len(tails) == len(heads)
        assert len(tails) == len(weights)
        assert len(tails) == self.graph.number_of_edges()

        return tails, heads, weights

    def _dump_array(self, dump: Path, array: List[int]) -> None:
        with dump.open("w") as f:
            f.write("\n".join(str(x) for x in array))
            f.write("\n")

    @staticmethod
    def osmid_to_osmurl(node_osmid):
        return "https://www.openstreetmap.org/node/{}".format(node_osmid)

    def clone_nodes(self) -> nx.MultiDiGraph:
        clone = nx.MultiDiGraph(**self.graph.graph)
        for node, data in self.graph.nodes(data=True):
            clone.add_node(node, **data)
        return clone
