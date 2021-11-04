#!/usr/bin/env python3

from enum import Enum, auto


class Vehicle(Enum):
    CAR = auto()
    MOTO = auto()
    BIKE = auto()

    def __str__(self) -> str:
        return self.name.lower()

    @property
    def description(self) -> str:
        return f"This vehicle is a {self.name.lower()}"


if __name__ == "__main__":
    print(
        """
    The purpose of this POC is to show how to implement an "enum of string".

    Three ways are illustrated :
        - using 'name' attribute
        - defining a custom '__str__' method
        - defining a custom 'description' method
    """
    )
    print(Vehicle.CAR.name)
    print(str(Vehicle.MOTO))
    print(Vehicle.BIKE.description)
