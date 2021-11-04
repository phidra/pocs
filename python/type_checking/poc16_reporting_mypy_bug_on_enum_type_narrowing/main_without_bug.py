#!/usr/bin/env python3

from enum import Enum, auto


class Planet(Enum):
    MERCURY = auto()
    VENUS = auto()


def get_gravity(x: Planet) -> float:
    reveal_type(x)  # noqa: F821
    if x is Planet.MERCURY:
        reveal_type(x)  # noqa: F821
        return 0.0
    reveal_type(x)  # noqa: F821
    return 1.0


if __name__ == "__main__":
    print("Gravity is : ", get_gravity(Planet.MERCURY))
    print("Gravity is : ", get_gravity(Planet.VENUS))
