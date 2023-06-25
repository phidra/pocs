import sys
from pathlib import Path
from contextlib import contextmanager
from dataclasses import dataclass, field
from typing import Tuple, List, Set, Optional

import osmnx
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.axes import Axes
from matplotlib.figure import Figure
from shapely.geometry import MultiPoint
from dijkstra import HandlerWantsToStopPropagation


def compute_hull(G, nodes) -> Tuple[List[float], List[float]]:
    multipoint = []
    for node in nodes:
        multipoint.append((G.nodes[node]["x"], G.nodes[node]["y"]))
    hull = MultiPoint(multipoint).convex_hull
    hull_lngs = [p[0] for p in hull.exterior.coords]
    hull_lats = [p[1] for p in hull.exterior.coords]
    return hull_lngs, hull_lats


class PlotConf:
    NO_NODE = {
        "node_size": 0,
    }
    LIGHT_EDGES = {
        "edge_color": "k",
        "edge_linewidth": 0.5,
    }
    RED_EDGES = {
        "edge_color": "r",
        "edge_linewidth": 3,
    }


@dataclass
class IncrementalDrawer:
    """
    General drawer, that progressively draws something on a main ax.
    It is aimed to save an image at each step of a progression (typically : Dijkstra propagation)

    Once something is drawn on the main ax, it will stay there for ALL the future saved images.

    In contrast, it is possible to draw some thing only for a single saved image (e.g. the hull of current nodes, or
    the current number of settled nodes). These "temporary" drawings are made on temporary axes, that are removed
    after each save.
    """

    output_dir: Path
    file_base_name: str
    save_period: int  # will save one image every 'plot_period' repetition
    fig: Figure = field(init=False)
    main_ax: Axes = field(init=False)  # this is the main ax (it will be REUSED for all the saved images)
    temporary_axes: Set[Axes] = field(default_factory=set)  # each of these axes is used only for a SINGLE saved image
    file_counter: int = 1
    max_nb_file: int = sys.maxsize
    accumulator: int = 0
    drawing_bbox: Optional[Tuple[float, float, float, float]] = None  # North, South, East, West

    def __post_init__(self):
        self.fig, self.main_ax = plt.subplots(figsize=(12.80, 10.24))
        self.fig.tight_layout(pad=0, h_pad=0, w_pad=0)
        size_in_pixels = self.fig.get_size_inches() * self.fig.dpi
        print("SIZE INCHES = ", self.fig.get_size_inches())
        print("SIZE DPI = ", self.fig.dpi)
        print("FIGSIZE = ", size_in_pixels)
        print("type(FIGSIZE) = ", type(size_in_pixels).__name__)  # ndarray

        # negative values indicates that handler must not stop dijkstra :
        if self.max_nb_file <= 0:
            self.max_nb_file = sys.maxsize

    def on_call(self):
        self.accumulator += 1

    def begin_new_repetition(self):
        self.file_counter += 1  # bump next file to write
        while self.temporary_axes:
            temp_ax = self.temporary_axes.pop()
            temp_ax.remove()
        self.accumulator = 0

    def should_save(self):
        return self.accumulator >= self.save_period

    def get_temp_ax(self):
        temp_ax = self.add_new_ax()
        self.temporary_axes.add(temp_ax)
        return temp_ax

    @contextmanager
    def keep_axes_limits(self):
        """ This decorator puts back proper limits (after a small-scale drawing, that messes up the ax limits) """
        xlim_before_second_drawing = self.main_ax.get_xlim()
        ylim_before_second_drawing = self.main_ax.get_ylim()
        try:
            yield
        finally:
            self.main_ax.set_xlim(xlim_before_second_drawing)
            self.main_ax.set_ylim(ylim_before_second_drawing)

    def add_new_ax(self):
        """ adds a new ax, and configures it to properly integrate with existing figure"""
        new_ax = self.fig.add_axes(self.main_ax.get_position(), sharex=self.main_ax, sharey=self.main_ax)
        new_ax.patch.set_alpha(0)  # background of new axes must be transparent

        # inspired by osmnx.plot.py:_config_ax :
        new_ax.margins(0)
        for s in new_ax.spines.values():
            s.set_visible(False)
        new_ax.get_xaxis().set_visible(False)
        new_ax.get_yaxis().set_visible(False)
        return new_ax

    def draw_temporary_text(self, text):
        """ Draw texts (at the top-left corner, hardcoded for now) """
        # https://matplotlib.org/stable/api/_as_gen/matplotlib.axes.Axes.text.html#matplotlib.axes.Axes.text
        if text is None:
            return
        text_ax = self.get_temp_ax()
        text_ax.text(0.02, 0.9, text, transform=self.main_ax.transAxes)
        return text_ax

    def draw_temporary_hull(self, hull):
        if hull is None:
            return
        hull_lngs, hull_lats = hull
        hull_ax = self.get_temp_ax()
        hull_ax.fill(hull_lngs, hull_lats, color=(0, 0, 1, 0.3))

    @property
    def filename(self) -> Path:
        return self.output_dir / "{}-{:04d}.png".format(self.file_base_name, self.file_counter)

    def save_figure(self):
        self.fig.savefig(self.filename)
        print("ADDING EDGE AND DUMPING IN : ", self.filename)

    def plot_graph(self, graph, **options):
        options_to_use = options.copy()
        if "ax" not in options_to_use:
            options_to_use["ax"] = self.main_ax
        if "show" not in options_to_use:
            options_to_use["show"] = False
        if "bbox" not in options_to_use:
            options_to_use["bbox"] = self.drawing_bbox
        osmnx.plot_graph(graph, **options_to_use)


@dataclass
class DijkstraDrawer(IncrementalDrawer):
    """ This is a drawer that knows how to react to dijkstra-events, by plotting dijkstra propagation. """

    nodes_to_draw: Set[int] = field(default_factory=set)  # settled nodes that are not yet plotted
    edges_to_draw: Set[Tuple[int, int]] = field(default_factory=set)  # settled edges that are not yet plotted

    def begin_new_repetition(self):
        IncrementalDrawer.begin_new_repetition(self)
        self.nodes_to_draw = set()
        self.edges_to_draw = set()

    def on_node_settled(self, dijkstra, currently_settled_node):
        its_parent = dijkstra.parents[currently_settled_node]
        if currently_settled_node == its_parent:
            return
        self.nodes_to_draw.add(currently_settled_node)
        self.nodes_to_draw.add(its_parent)
        self.edges_to_draw.add((its_parent, currently_settled_node))

        self.on_call()
        if self.should_save():
            text_to_draw = f"nb settled nodes = {len(dijkstra.settled_nodes)}"
            subgraph = self.get_subgraph_to_draw(dijkstra.G)
            self.add_subgraph_and_save(subgraph, text_to_draw=text_to_draw)
            self.begin_new_repetition()

        if self.file_counter > self.max_nb_file:
            raise HandlerWantsToStopPropagation

    def get_subgraph_to_draw(self, G):
        subgraph = nx.MultiDiGraph(**G.graph)
        for node in sorted(self.nodes_to_draw):
            subgraph.add_node(node, **G.nodes[node])
        for tail, head in sorted(self.edges_to_draw):
            for _, edge_data in G.adj[tail][head].items():
                subgraph.add_edge(tail, head, **edge_data)
        return subgraph

    def add_subgraph_and_save(self, subgraph, hull_to_draw=None, text_to_draw=None):
        """
        On the main ax, on which there might already be some drawings (e.g. the previous state of Dijkstra propagation)
        draws additional things (e.g. nodes+edges+text_to_draw+hull), then save figure.
        """

        # drawing new edges (in red) on main ax :
        with self.keep_axes_limits():
            self.plot_graph(subgraph, **PlotConf.NO_NODE, **PlotConf.RED_EDGES)

        # drawing "temporary" axes (the ones that are remove+redrawn at each iteration) :
        self.draw_temporary_hull(hull_to_draw)
        self.draw_temporary_text(text_to_draw)
        self.save_figure()
