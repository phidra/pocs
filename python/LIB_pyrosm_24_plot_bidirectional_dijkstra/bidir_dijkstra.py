import sys
from typing import List, Dict, Set, TYPE_CHECKING, Optional, Any
from dataclasses import dataclass, field

from heapq import heappush, heappop

if TYPE_CHECKING:
    import networkx as nx

from dijkstra import HandlerWantsToStopPropagation


@dataclass
class DijkstraState:
    parents: Dict[int, int] = field(default_factory=dict)  # key=osmid / value=osmid
    costs: Dict[int, int] = field(default_factory=dict)  # key=osmid / value=cost
    settled_nodes: Set[int] = field(default_factory=set)  # key=osmid
    queue: List[int] = field(default_factory=list)  # this is actually a heap (storing osmid)...
    currently_settled_node: Optional[int] = None

    def reset(self, graph, origin):
        self.G = graph  # FIXME, this is an alias, badly handled...
        self.costs = {}
        for node in graph.nodes():
            self.costs[node] = sys.maxsize  # initial costs are huge
        self.costs[origin] = 0  # origin cost is null
        self.settled_nodes = set()
        self.parents = {origin: origin}
        self.queue = []
        self.currently_settled_node = None
        heappush(self.queue, (0, origin))

    def reconstruct_path(self, source, joining_node):
        """ From the 'parents' structure filled by dijkstra, reconstruct the full path. """
        path = []  # warning : the path is initially reconstructed backwards, then reversed

        parent = joining_node
        while parent != source:
            path.append(parent)
            parent = self.parents[parent]
        path.append(source)
        path.reverse()
        return path

    def relax(self, graph, node, weight_attr):
        relaxed_cost = self.costs[node]
        for successor, multiedge_properties in graph[node].items():
            if successor in self.settled_nodes:
                continue
            edge_data = multiedge_properties[0]  # for the sake of simplicity, we ignore parallel edges

            successor_old_cost = self.costs[successor]
            successor_new_cost = relaxed_cost + edge_data[weight_attr]

            if successor_new_cost < successor_old_cost:
                self.costs[successor] = successor_new_cost
                self.parents[successor] = node
                heappush(self.queue, (successor_new_cost, successor))
                # note : this might insert duplicates, which is ok as all but one (the one with the smallest weight)
                # will be later ignored as already settled

    def settle_next_node(self, graph, weight_attr):
        # FIXME : handle breaking condition here
        if self.queue:
            _, self.currently_settled_node = heappop(self.queue)
            self.settled_nodes.add(self.currently_settled_node)
            self.relax(graph, self.currently_settled_node, weight_attr)
            return self.currently_settled_node
        return None


@dataclass
class BidirDijkstra:
    forward_graph: "nx.MultiDiGraph"
    backward_graph: "nx.MultiDiGraph"
    event_handler_forward: Any
    event_handler_backward: Any
    weight_attr: str

    should_propagate_forward: bool = True
    meeting_nodes: Set[int] = field(default_factory=set)
    forward_state: DijkstraState = field(default_factory=DijkstraState)
    backward_state: DijkstraState = field(default_factory=DijkstraState)

    def init(self, source, target):
        self.forward_state.reset(self.forward_graph, source)
        self.backward_state.reset(self.backward_graph, target)
        self.should_propagate_forward = True

    def run(self, source, target):
        self.init(source, target)

        # FIXME : gérer avec itertools + exception ?
        try:
            while not self.meeting_nodes:
                self.should_propagate_forward = not self.should_propagate_forward
                if self.should_propagate_forward:
                    settled_node = self.forward_state.settle_next_node(self.forward_graph, self.weight_attr)
                    if settled_node is None:
                        continue
                    self.event_handler_forward.on_forward_node_settled(self, settled_node)
                    if settled_node in self.backward_state.settled_nodes:
                        self.meeting_nodes.add(settled_node)
                else:
                    settled_node = self.backward_state.settle_next_node(self.backward_graph, self.weight_attr)
                    if settled_node is None:
                        continue
                    self.event_handler_backward.on_backward_node_settled(self, settled_node)
                    if settled_node in self.forward_state.settled_nodes:
                        self.meeting_nodes.add(settled_node)
        except HandlerWantsToStopPropagation:
            return []  # in this case, we are not interested in the path

        # here, a meeting node has been found, but it is NOT necessarily on the shortest path !
        # We must find the node on the shortest path (a node that is reached by both sides, with minimal sum_cost)
        # To distinguish it from the meeting node, we call it the "joining" node.
        # FIXME : there is probably a more efficient implementation
        joining_cost_so_far = sys.maxsize
        joining_node_so_far = None
        for node, forward_cost in self.forward_state.costs.items():
            backward_cost = self.backward_state.costs[node]
            sum_cost = forward_cost + backward_cost
            if sum_cost < joining_cost_so_far:
                joining_node_so_far = node
                joining_cost_so_far = sum_cost
        assert (joining_node_so_far is not None)

        forward_path = self.forward_state.reconstruct_path(source, joining_node_so_far)
        backward_path = self.backward_state.reconstruct_path(target, joining_node_so_far)
        backward_path.reverse()  # FIXME = describe
        if backward_path:
            return forward_path + backward_path[1:]  # exclude one joining node
        else:
            return forward_path
