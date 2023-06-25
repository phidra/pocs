#!/usr/bin/env python3

import sys

"""
This version sometimes fails, because it assumes that balances are declared
in the order when they appears in the stack.

In other words, it assumes that reverse traversing the balances-array is
sufficient to traverse the stak from top to bottom.

It is not always the case, as demonstrates test #3
"""


class Side:
    LEFT = "left"
    RIGHT = "right"


class GlobalState:

    BASE_BALANCE_MASS = 10

    def __init__(self, fpath):
        self.initial_weights = []
        self.stacked_balances = []
        self.added_weights = []
        self.nb_balances = 0
        self.parse(fpath)
        self.sanity_check()

    def parse(self, fpath):
        with open(fpath, "r") as f:
            lines = [x.rstrip("\n") for x in f.readlines() if not x.startswith("#")]
        self.nb_balances = int(lines[0])
        assert len(lines) == 2 * self.nb_balances + 1
        for line in lines[1:]:
            tokens = [int(x) for x in line.rstrip("\n").split(" ")]
            self.initial_weights.append(tokens[0])
            self.stacked_balances.append(tokens[1:])
        self.added_weights = [0] * (self.nb_balances * 2)
        # values for the example given in the instructions :
        # nb_balances      = 4
        # initial_weights  = [0, 0, 0, 0, 3, 0, 0, 0]
        # stacked_balances = [[1], [2], [], [3], [], [], [], []]
        # added_weights    = [0, 0, 0, 0, 0, 0, 0, 0]

    def sanity_check(self):
        for struct in (self.initial_weights, self.stacked_balances, self.added_weights):
            assert(len(struct) == 2 * self.nb_balances)

    def total_mass(self, balance):
        return self.BASE_BALANCE_MASS + self.mass(Side.LEFT, balance) + self.mass(Side.RIGHT, balance)

    def mass(self, side, balance):
        return (
            self.get(side, balance, self.initial_weights) +
            self.get(side, balance, self.added_weights) +
            sum([self.total_mass(b) for b in self.get(side, balance, self.stacked_balances)])
        )

    def add_weight(self, side, balance, added_weight):
        self.added_weights[self.idx(side, balance)] = added_weight

    @staticmethod
    def idx(side, balance):
        if side == Side.LEFT:
            return 2 * balance
        return 2 * balance + 1

    @staticmethod
    def get(side, balance, struct):
        return struct[GlobalState.idx(side, balance)]


def run(fpath):
    s = GlobalState(fpath)

    # iterate over balances from the end, and add weights if necessary :
    # EDIT : iterating over balances from the end of the array is not
    #        sufficient to guarantee to traverse the stack from top
    #        to bottom -> this is buggy.
    for balance in range(s.nb_balances)[::-1]:
        diff = s.mass(Side.RIGHT, balance) - s.mass(Side.LEFT, balance)
        if diff > 0:
            s.add_weight(Side.LEFT, balance, diff)
        elif diff < 0:
            s.add_weight(Side.RIGHT, balance, -diff)

    # at this step, balances are even :
    output = []
    for balance in range(s.nb_balances):
        assert(s.mass(Side.LEFT, balance) == s.mass(Side.RIGHT, balance))
        output.append("{}: {} {}".format(
            balance,
            GlobalState.get(Side.LEFT, balance, s.added_weights),
            GlobalState.get(Side.RIGHT, balance, s.added_weights),
        ))
    return output


if __name__ == "__main__":
    for line in run(sys.argv[1]):
        print(line)
