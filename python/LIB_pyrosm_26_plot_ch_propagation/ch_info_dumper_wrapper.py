import subprocess
from pathlib import Path
from typing import List, Iterator, Union
from dataclasses import dataclass, field, fields
from enum import Enum, unique, auto

from original_graph import OriginalGraph


@unique
class Side(Enum):
    FORWARD = auto()
    BACKWARD = auto()


def parse_integer_file(f_path: str, expected_suffix: str) -> List[int]:
    assert f_path.endswith(expected_suffix)
    with open(f_path, "r") as f:
        return [int(line) for line in f]


def array_summary(field_name: str, array: Union[List[int], List[bool]], limit: int = 5) -> str:
    first_items_string = " ".join(str(x) for x in array[:limit])
    return "{} (size={}) first items = {}".format(field_name, len(array), first_items_string)


@dataclass
class ChSideInfos:
    """ This class stores the raw infos given by ch dumper for a given side."""
    side: Side
    first_out: List[int] = field(default_factory=list)
    head: List[int] = field(default_factory=list)
    weight: List[int] = field(default_factory=list)
    shortcut_first_arc: List[int] = field(default_factory=list)
    shortcut_second_arc: List[int] = field(default_factory=list)
    is_shortcut_an_original_arc: List[bool] = field(default_factory=list)

    @staticmethod
    def create_from_file_list(side: Side, files: Iterator[str]) -> "ChSideInfos":
        """
        Parse a list of files to get a CH side (forward or backward) infos.
        The files are expected to be in a precise order.
        Example of input file list :
            - NOGIT_OUT_forward_first_out.txt
            - NOGIT_OUT_forward_head.txt
            - NOGIT_OUT_forward_weight.txt
            - NOGIT_OUT_forward_shortcut_first_arc.txt
            - NOGIT_OUT_forward_shortcut_second_arc.txt
            - NOGIT_OUT_forward_is_shortcut_an_original_arc.txt
        """
        data = ChSideInfos(side)
        data.first_out = parse_integer_file(next(files), "first_out.txt")
        data.head = parse_integer_file(next(files), "head.txt")
        data.weight = parse_integer_file(next(files), "weight.txt")
        data.shortcut_first_arc = parse_integer_file(next(files), "shortcut_first_arc.txt")
        data.shortcut_second_arc = parse_integer_file(next(files), "shortcut_second_arc.txt")
        # is_shortcut_an_original_arc needs an additional conversion to bool :
        temporary = parse_integer_file(next(files), "is_shortcut_an_original_arc.txt")
        data.is_shortcut_an_original_arc = [bool(x) for x in temporary]
        return data

    def field_summary(self, field_name: str, limit: int = 5) -> str:
        """ Returns a summary string describing one of the class' field. """
        array = getattr(self, field_name)
        return array_summary(field_name, array, limit)

    def summary(self) -> str:
        excluded_fields = ["side"]
        field_names = [f.name for f in fields(self) if f.name not in excluded_fields]
        to_return = f"Side = {self.side} :\n"
        to_return += "\n".join(self.field_summary(field) for field in field_names)
        to_return += "\n"
        return to_return


@dataclass
class RawContractionHierarchy:
    ranks: List[int]
    orders: List[int]
    forward: ChSideInfos
    backward: ChSideInfos

    @staticmethod
    def create_from_file_list(files: Iterator[str]) -> "RawContractionHierarchy":
        """
        Parse a list of files (typically, produced by C++ code) to get a CH "raw" infos.
        ("raw" in opposition to an interpretation of these info, to build forward/backward graphs from raw infos)
        The files are expected to be in a precise order.
        Example of input file list :
            - NOGIT_OUT_ranks.txt
            - NOGIT_OUT_orders.txt
            - NOGIT_OUT_forward_first_out.txt
            - NOGIT_OUT_forward_head.txt
            - NOGIT_OUT_forward_weight.txt
            - NOGIT_OUT_forward_shortcut_first_arc.txt
            - NOGIT_OUT_forward_shortcut_second_arc.txt
            - NOGIT_OUT_forward_is_shortcut_an_original_arc.txt
            - NOGIT_OUT_backward_first_out.txt
            - NOGIT_OUT_backward_head.txt
            - NOGIT_OUT_backward_weight.txt
            - NOGIT_OUT_backward_shortcut_first_arc.txt
            - NOGIT_OUT_backward_shortcut_second_arc.txt
            - NOGIT_OUT_backward_is_shortcut_an_original_arc.txt
        """
        print("")
        print("")
        print("")
        print("~~~~~~~~~ liste de fichiers = ~~~~~~~~~~~~~~~~")
        li = list(files)
        for f in li:
            print("\t ", f)
        print("")
        print("")
        print("")
        files = iter(li)
        ranks = parse_integer_file(next(files), "_ranks.txt")
        orders = parse_integer_file(next(files), "_orders.txt")
        forward = ChSideInfos.create_from_file_list(Side.FORWARD, files)
        backward = ChSideInfos.create_from_file_list(Side.BACKWARD, files)
        return RawContractionHierarchy(ranks=ranks, orders=orders, forward=forward, backward=backward)

    def summary(self) -> str:
        to_return = "\n".join([
            array_summary("ranks", self.ranks),
            array_summary("orders", self.orders),
            self.forward.summary(),
            self.backward.summary(),
        ])
        return to_return

    def _unpack_forward_arc(self, arc, callback):
        if(self.forward.is_shortcut_an_original_arc[arc]):
            callback(self.forward.shortcut_first_arc[arc], self.forward.shortcut_second_arc[arc])
        else:
            assert(self.forward.shortcut_first_arc[arc] < len(self.backward.head))
            assert(self.forward.shortcut_second_arc[arc] < len(self.forward.head))
            self._unpack_backward_arc(self.forward.shortcut_first_arc[arc], callback)
            self._unpack_forward_arc(self.forward.shortcut_second_arc[arc], callback)

    def _unpack_backward_arc(self, arc, callback):
        if(self.backward.is_shortcut_an_original_arc[arc]):
            callback(self.backward.shortcut_first_arc[arc], self.backward.shortcut_second_arc[arc])
        else:
            assert(self.backward.shortcut_first_arc[arc] < len(self.backward.head))
            assert(self.backward.shortcut_second_arc[arc] < len(self.forward.head))
            self._unpack_backward_arc(self.backward.shortcut_first_arc[arc], callback)
            self._unpack_forward_arc(self.backward.shortcut_second_arc[arc], callback)

    def _unpack_shortcut(self, edge_index, first_unpacker):
        """
        WARNING 1 : the returned nodes are the node's ORDER (and not the node ranks, as one would expect...)
        WARNING 2 : from a shortcut AB, this returns all nodes from A to B... EXCEPT A !

        NOTE : this functions works fine even if the arc is not a shortcut but an original arc.
        """
        path_as_node_orders = []
        callback = lambda _, node_rank: path_as_node_orders.append(node_rank)  # noqa: E731
        first_unpacker(edge_index, callback)
        return path_as_node_orders

    def unpack_forward_shortcut(self, edge_index):
        return self._unpack_shortcut(edge_index, first_unpacker=self._unpack_forward_arc)

    def unpack_backward_shortcut(self, edge_index):
        return self._unpack_shortcut(edge_index, first_unpacker=self._unpack_backward_arc)


def build_ch(files: OriginalGraph.DumpedFiles, output_dir: Path) -> RawContractionHierarchy:
    binary = "./NOGIT_ch_info_dumper"
    env = {
        "LD_LIBRARY_PATH": "DEPENDENCY_ch_info_dumper/NOGIT_RoutingKit_repository/lib",
    }
    args = [
        binary,
        str(files.tailsfile),
        str(files.headsfile),
        str(files.weightsfile),
        str(files.osmidsfile),
        str(output_dir),
    ]

    print("About to call {} with these args :".format(binary))
    for arg in args:
        print("\t", arg)
    print("")

    completed_process = subprocess.run(args, capture_output=True, check=True, text=True, env=env)

    print("Logs of the call :")
    print(completed_process.stderr)
    print("")

    output = filter(None, completed_process.stdout.split("\n"))
    ch = RawContractionHierarchy.create_from_file_list(output)
    return ch
