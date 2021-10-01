#!/usr/bin/env python3

import abc


class Interface1_Simple:  # abc.ABCMeta is NOT in this interface's metaclasses
    @abc.abstractmethod
    def do_a_barrel_roll(self):
        raise NotImplementedError


class Interface2_WithAbcMeta(metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def do_a_barrel_roll(self):
        raise NotImplementedError


class Interface3_WithAbc(abc.ABC):
    @abc.abstractmethod
    def do_a_barrel_roll(self):
        raise NotImplementedError


class Plane1(Interface1_Simple):
    pass


class Plane2(Interface2_WithAbcMeta):
    pass


class Plane3(Interface3_WithAbc):
    pass


poc_description = """
This POC shows that in order for @abc.abstractmethod to PREVENT instanciation of bugged subclasses, one must use abc.ABCMeta or
abc.ABC in the interface that defines an abstractmethod :

    - 3 interfaces are defined, they are (almost) identical, with exactly the same abstractmethod :
        Interface1 doesn't use abc.ABCMeta / abc.ABC
        Interface2 has abc.ABCMeta in its metaclasses
        Interface3 inherits from abc.ABC (thus has indirectly abc.ABCMeta in its metaclasses)
    - for each interface, a concrete class is derived, with a bug : it DOES NOT implement the required abstractmethod
    - the goal of the POC is to see if instanciation of these concrete classes are forbidden AT RUNTIME
    - (the behaviour at type-checking-time is clear : mypy (rightfully) complains in all three cases)

CONCLUSION : the instanciation is prohibited only if abc.ABCMeta (or abc.ABC) is used.

EXPECTED OUTPUT :

    Traceback (most recent call last):
      File "./poc.py", line 80, in <module>
          plane3 = Plane3()
    TypeError: Can't instantiate abstract class Plane3 with abstract methods do_a_barrel_roll

"""  # noqa: E501

print(poc_description)


# instanciating Plane1 IS NOT forbidden at runtime (because Interface1 does not use ABCMeta / ABC) :
plane1 = Plane1()


# instanciating Plane2 IS forbidden at runtime (TypeError is raised) :
try:
    plane2 = Plane2()
    assert False  # just proving that we do NOT pass here (because the previous line raised TypeError)
except TypeError:
    pass  # swallowing the exception here, to see what happens with plane3


# instanciating Plane3 IS forbidden at runtime (TypeError is raised) :
plane3 = Plane3()


print("If you see this message, it's because at runtime, all was OK (which is NOT what was expected !)")
# NOTE : for all three instanciations, mypy (rightfully) complains :-)
