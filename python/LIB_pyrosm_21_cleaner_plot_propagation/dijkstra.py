import sys
from typing import List, Dict, Set, TYPE_CHECKING, Any
from dataclasses import dataclass, field

from heapq import heappush, heappop

if TYPE_CHECKING:
    import networkx as nx


@dataclass
class Dijkstra:
    G: "nx.MultiDiGraph"
    weight_attr: str
    event_handler: Any

    # dijkstra state :
    parents: Dict[int, int] = field(default_factory=dict)  # key=osmid / value=osmid
    costs: Dict[int, int] = field(default_factory=dict)  # key=osmid / value=cost
    settled_nodes: Set[int] = field(default_factory=set)  # key=osmid
    queue: List[int] = field(default_factory=list)  # this is actually a heap (storing osmid)...

    def reconstruct_path(self, source, target):
        """ From the 'parents' structure filled by dijkstra, reconstruct the full path. """
        path = []  # warning : the path is initially reconstructed backwards, then reversed

        parent = target
        while parent != source:
            path.append(parent)
            parent = self.parents[parent]
        path.append(source)
        path.reverse()
        return path

    def relax(self, node):
        relaxed_cost = self.costs[node]
        for successor, multiedge_properties in self.G[node].items():
            if successor in self.settled_nodes:
                continue
            edge_data = multiedge_properties[0]  # for the sake of simplicity, we ignore parallel edges

            successor_old_cost = self.costs[successor]
            successor_new_cost = relaxed_cost + edge_data[self.weight_attr]

            if successor_new_cost < successor_old_cost:
                self.costs[successor] = successor_new_cost
                self.parents[successor] = node
                heappush(self.queue, (successor_new_cost, successor))
                # note : this might insert duplicates, which is ok as all but one (the one with the smallest weight)
                # will be later ignored as already settled

    def init(self, source):
        self.costs = {}
        for node in self.G.nodes():
            self.costs[node] = sys.maxsize  # initial costs are huge
        self.costs[source] = 0  # source cost is null
        self.settled_nodes = set()
        self.parents = {source: source}
        self.queue = []
        heappush(self.queue, (0, source))

    def run(self, source, target):
        self.init(source)
        currently_settled_node = None
        while self.queue and currently_settled_node != target:
            _, currently_settled_node = heappop(self.queue)
            self.settled_nodes.add(currently_settled_node)
            self.relax(currently_settled_node)
            self.event_handler.on_node_settled(self, currently_settled_node)

        return self.reconstruct_path(source, target)
