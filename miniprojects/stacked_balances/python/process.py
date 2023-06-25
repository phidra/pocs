#!/usr/bin/env python3

import sys
import itertools


class Side:
    LEFT = "left"
    RIGHT = "right"


class BalancePlate:
    def __init__(self, initial_weight, balances):
        self.initial_weight = initial_weight
        self.balances = balances
        self.added_weight = 0

    def mass(self):
        return self.initial_weight + self.added_weight + sum([b.mass() for b in self.balances])

    def add_weight(self, weight):
        self.added_weight += weight


class Balance:
    BASE_BALANCE_MASS = 10

    def __init__(self, left_plate, right_plate):
        self.left_plate = left_plate
        self.right_plate = right_plate

    def add_left_weight(self, weight):
        self.left_plate.add_weight(weight)

    def add_right_weight(self, weight):
        self.right_plate.add_weight(weight)

    def mass(self):
        return self.BASE_BALANCE_MASS + self.left_plate.mass() + self.right_plate.mass()

    def equilibrate(self):
        for sub_balance in itertools.chain(self.left_plate.balances, self.right_plate.balances):
            sub_balance.equilibrate()

        # once all children are even, perform self equilibrate :
        diff = self.right_plate.mass() - self.left_plate.mass()
        if diff > 0:
            self.add_left_weight(diff)
        elif diff < 0:
            self.add_right_weight(-diff)

        assert self.left_plate.mass() == self.right_plate.mass()


def parse(fpath):
    with open(fpath, "r") as f:
        lines = [x.rstrip("\n") for x in f.readlines() if not x.startswith("#")]
    nb_balances = int(lines[0])
    assert len(lines) == 2 * nb_balances + 1
    plates = []
    for plate_infos in [l.rstrip("\n").split(" ") for l in lines[1:]]:
        initial_weight = int(plate_infos[0])
        stacked_balances = [int(x) for x in plate_infos[1:]]
        plates.append(BalancePlate(initial_weight, stacked_balances))

    balances = []
    for left_plate, right_plate in zip(plates[0::2], plates[1::2]):
        balances.append(Balance(left_plate, right_plate))

    # replace balance indexes with balance objects :
    for balance in balances:
        left_balances = [balances[i] for i in balance.left_plate.balances]
        right_balances = [balances[i] for i in balance.right_plate.balances]
        balance.left_plate.balances = left_balances
        balance.right_plate.balances = right_balances

    return balances


def run(fpath):
    balances = parse(fpath)
    balances[0].equilibrate()

    # at this step, balances are even

    output = []
    for idx, balance in enumerate(balances):
        assert(balance.left_plate.mass() == balance.right_plate.mass())
        output.append("{}: {} {}".format(
            idx,
            balance.left_plate.added_weight,
            balance.right_plate.added_weight,
        ))
    return output


if __name__ == "__main__":
    for line in run(sys.argv[1]):
        print(line)
