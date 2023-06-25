from dataclasses import dataclass, field
from typing import List, Dict

from shapely.geometry import LineString
from shapely.ops import linemerge
import networkx as nx

from ch_info_dumper_wrapper import RawContractionHierarchy, ChSideInfos, Side
from original_graph import OriginalGraph


def get_edge_geometry(original_graph, path):
    """ From an edge path (i.e. a sequence of node's osmid) that might be a shortcut, returns its geometry. """

    lines = []
    for tail, head in zip(path[:-1], path[1:]):
        try:
            original_edge_data = original_graph[tail][head][0]
        except KeyError as e:
            print("")
            print("EXCEPTION")
            print("EXCEPTION")
            print("EXCEPTION")
            print(f"tail>  osmid={tail}    osmurl=https://www.openstreetmap.org/node/{tail}")
            print(f"head>  osmid={head}    osmurl=https://www.openstreetmap.org/node/{head}")
            print("")
            raise(e)
        try:
            edge_geom = original_edge_data["geometry"]
        except KeyError:
            tail_geom = original_graph.nodes[tail]["geometry"]
            head_geom = original_graph.nodes[head]["geometry"]
            edge_geom = LineString([tail_geom, head_geom])
        lines.append(edge_geom)
    return linemerge(lines)


@dataclass
class Conversion:
    """
    This class allows to convert to/from various representation of a node :
        - its osmid  (this is what identifies a node in a networkx' graph)
        - its order (this is what identifies a node at the input of RoutingKit code)
        - its rank (this is what identifies a node inside RoutingKit's CH code)
    """
    order_to_rank: List[int] = field(default_factory=list)
    order_to_osmid: List[int] = field(default_factory=list)
    rank_to_order: List[int] = field(default_factory=list)
    rank_to_osmid: List[int] = field(default_factory=list)
    osmid_to_order: Dict[int, int] = field(default_factory=dict)
    osmid_to_rank: Dict[int, int] = field(default_factory=dict)

    @staticmethod
    def create(raw_ch: RawContractionHierarchy, orig_graph: OriginalGraph) -> "Conversion":
        order_to_rank = raw_ch.ranks
        order_to_osmid = orig_graph.orders
        rank_to_order = raw_ch.orders
        rank_to_osmid = [order_to_osmid[rank_to_order[rank]] for rank in range(orig_graph.graph.number_of_nodes())]
        osmid_to_order = {osmid: orig_graph.graph.nodes[osmid]["order"] for osmid in orig_graph.graph.nodes()}
        osmid_to_rank = {osmid: order_to_rank[osmid_to_order[osmid]] for osmid in orig_graph.graph.nodes()}
        return Conversion(
            order_to_rank,
            order_to_osmid,
            rank_to_order,
            rank_to_osmid,
            osmid_to_order,
            osmid_to_rank,
        )

    def _check_consistency(self):
        assert(len(self.order_to_rank) >= 43)
        testrank = 42
        rank1 = self.osmid_to_rank[self.rank_to_osmid[testrank]]
        rank2 = self.order_to_rank[self.rank_to_order[testrank]]
        assert(testrank == rank1)
        assert(testrank == rank2)

        order1 = self.rank_to_order[testrank]
        order2 = self.osmid_to_order[self.rank_to_osmid[testrank]]
        assert(order1 == order2)

        osmid1 = self.rank_to_osmid[testrank]
        osmid2 = self.order_to_osmid[self.rank_to_order[testrank]]
        assert(osmid1 == osmid2)


@dataclass
class ContractionHierarchy:
    forward: nx.MultiDiGraph  # contracted graph
    backward: nx.MultiDiGraph  # contracted graph
    conversion: Conversion

    @staticmethod
    def create(raw_ch: RawContractionHierarchy, orig_graph: OriginalGraph) -> "ContractionHierarchy":
        conversion = Conversion.create(raw_ch, orig_graph)
        forward_graph = ContractionHierarchy._create_side_graph(
            raw_ch.forward,
            raw_ch.unpack_forward_shortcut,
            orig_graph,
            conversion,
        )
        print("=================")
        print("on passe à la construction du side graph backward")
        backward_graph = ContractionHierarchy._create_side_graph(
            raw_ch.backward,
            raw_ch.unpack_backward_shortcut,
            orig_graph,
            conversion,
        )
        ch = ContractionHierarchy(
            forward_graph,
            backward_graph,
            conversion,
        )
        return ch

    @staticmethod
    def _create_side_graph(side: ChSideInfos, unpacker, orig_graph: OriginalGraph, conv: Conversion) -> nx.MultiDiGraph:
        """ From CH infos on a given side (forward or backward), creates the networkx graph that represents it. """
        graph_to_return = orig_graph.clone_nodes()
        for osmid in graph_to_return.nodes():
            if side.side == Side.BACKWARD:
                # print(f"Adding edges of osmid = {osmid}")
                pass
            rank = conv.osmid_to_rank[osmid]
            edge_index_begin = side.first_out[rank]
            edge_index_end = side.first_out[rank+1]
            for edge_index in range(edge_index_begin, edge_index_end):
                head_rank = side.head[edge_index]
                head_osmid = conv.rank_to_osmid[head_rank]
                is_original = side.is_shortcut_an_original_arc[edge_index]
                weight = side.weight[edge_index]

                if side.side == Side.BACKWARD:
                    # print("EDGE BETWEEN :")
                    # print(f"    tail>  osmid={osmid}    osmurl=https://www.openstreetmap.org/node/{osmid}")
                    # print(f"    head>  osmid={head_osmid}    osmurl=https://www.openstreetmap.org/node/{head_osmid}")
                    # print("IS ORIGINAL ? ", is_original)
                    # print("")
                    pass

                if side.side == Side.FORWARD:
                    # The unpacked path is a sequence of osmid, starting with shortcut's tail, finishing with its head.
                    # If edge is original its length is exactly 2.
                    unpacked_path = [osmid]
                    for node_order in unpacker(edge_index):
                        unpacked_path.append(conv.order_to_osmid[node_order])
                else:
                    unpacked_path = [head_osmid]
                    for node_order in unpacker(edge_index):
                        unpacked_path.append(conv.order_to_osmid[node_order])
                    # print("unpacked path :")
                    for p in unpacked_path:
                        # print(f"    .... https://www.openstreetmap.org/node/{p}")
                        pass

                edge_geometry = get_edge_geometry(orig_graph.graph, unpacked_path)

                graph_to_return.add_edge(
                    osmid,
                    head_osmid,
                    weight=weight,
                    is_original=is_original,
                    is_shortcut=not is_original,
                    unpacked_path=unpacked_path,
                    geometry=edge_geometry,
                    length=edge_geometry.length,
                )
        return graph_to_return
