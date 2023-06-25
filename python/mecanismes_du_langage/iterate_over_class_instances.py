#!/usr/bin/env python3

from typing import List

print("""
This POC shows how to iterate over instances of a class

(by registering them in a _registry at __init__)

VERY inspired from :
    https://stackoverflow.com/questions/739882/iterating-over-object-instances-of-a-given-class-in-python

Notably, this reference has an interesting insight about use of metaclasses here :
    Putting iter in the Person class makes Person instances iterable,
    putting it in the metaclass makes the Person class itself iterable

--------------------
""")


class Person:
    _registry: List["Person"] = []

    def __init__(self, name: str):
        self.name = name
        Person._registry.append(self)

    def __str__(self):
        return f"Person({self.name})"


luke = Person("Luke")
leia = Person("Leia")
anakin = Person("Anakin")


print("The registered persons are :")
for person in Person._registry:
    print(f"   {person}")
print("")


# syntactic sugar = same effect, by iterating over Person2 (instead of Person._registry) :

class IterRegistry(type):
    def __iter__(cls):
        return iter(cls._registry)


class Person2(metaclass=IterRegistry):
    _registry: List["Person2"] = []

    def __init__(self, name: str):
        self.name = name
        Person2._registry.append(self)

    def __str__(self):
        return f"Person2({self.name})"


mario = Person2("Mario")
luigi = Person2("Luigi")
bowser = Person2("Bowser")


print("WITH METACLASSES : the registered persons are :")
for person in Person2:  # <-- NOTE : we iterate over the class itself here !
    print(f"   {person}")
print("")
