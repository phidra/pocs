#!/usr/bin/env python3

from enum import Enum


class Planet(Enum):
    MERCURY = (3.303e23, 2.4397e6)
    VENUS = (4.869e24, 6.0518e6)

    def __init__(self, mass, radius):
        self.mass = mass
        self.radius = radius

    @property
    def surface_gravity(self) -> float:
        G = 6.67300e-11
        return G * self.mass / (self.radius * self.radius)


def get_gravity(x: Planet) -> float:
    reveal_type(x)  # noqa: F821
    if x is Planet.MERCURY:
        reveal_type(x)  # noqa: F821
        return 0.0
    reveal_type(x)  # noqa: F821
    return x.surface_gravity


if __name__ == "__main__":
    print("Gravity is : ", get_gravity(Planet.MERCURY))
    print("Gravity is : ", get_gravity(Planet.VENUS))
