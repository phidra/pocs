#!/usr/bin/env python3

import abc


class PlaneInterface:  # no need to use abc.ABCMeta / abc.ABC here, see poc_description, and next POC
    @abc.abstractmethod
    def do_a_barrel_roll(self, name: str) -> str:
        raise NotImplementedError


class Wrong1_MissingMethod(PlaneInterface):
    def another_method(self):
        return 42

    # mypy will not complain here (as "Wrong1" could legitimally be a derived class that is still abstract)
    # BUT mypy will complain later when we try to instanciate this class


class Wrong2_ArgTypeMismatch(PlaneInterface):
    def do_a_barrel_roll(self, name: int) -> str:  # <-- mypy will (rightfully) complain here
        return "Look Ma, I'm doing a barrel-roll"


class Wrong3_ReturnTypeMismatch(PlaneInterface):
    def do_a_barrel_roll(self, name: str) -> int:  # <-- mypy will (rightfully) complain here
        return 42


class Correct(PlaneInterface):
    def do_a_barrel_roll(self, name: str) -> str:
        return "Look Ma, I'm doing a barrel-roll"


poc_description = """
This POC shows that @abc.abstractmethod helps mypy to detect some bugs :
    - we define a 'PlaneInterface' with type-annotated method
    - we derive 4 concrete implementations from this interfaces
    - 3 of them contains bugs (abstract method not implemented, wrong arg type, wrong return type)
    - the POC show that mypy catch all these 3 bugs.
    - (note : for the missing method, it is caught at instanciation time)

Note on ABCMeta :
    - usually, @abc.abstractmethod PREVENTS a class that fails to implemented required method to be instanciated
    - BUT as the python doc (quite unclearly) states, the use of abc.ABCMeta (or abc.ABC) is required for that :
        Using this decorator requires that the class’s metaclass is ABCMeta or is derived from it.
        (see https://docs.python.org/3/library/abc.html#abc.abstractmethod )
    - and in this poc, we ONLY use @abc.abstractmethod : the interface does not use abc.ABCMeta or abc.ABC
    - consequence : in this POC, instanciating the "wrong" children classes succeeds.
    - this behavious might be acceptable (e.g. if we rely on mypy to enforce the check), if it is not, i.e. if one
      actually wants the instanciation to fail at runtime, one just has to use abc.ABCMeta or abc.ABC) in the interface

EXPECTED RESULT = runtime runs smoothly (we see the message) + mypy produce the following output :

    poc.py:20: error: Argument 1 of "do_a_barrel_roll" is incompatible with supertype "PlaneInterface"; supertype defines the argument type as "str"
    poc.py:20: note: This violates the Liskov substitution principle
    poc.py:20: note: See https://mypy.readthedocs.io/en/stable/common_issues.html#incompatible-overrides
    poc.py:25: error: Return type "int" of "do_a_barrel_roll" incompatible with return type "str" in supertype "PlaneInterface"
    poc.py:65: error: Cannot instantiate abstract class 'Wrong1_MissingMethod' with abstract attribute 'do_a_barrel_roll'
    Found 3 errors in 1 file (checked 1 source file)

"""  # noqa: E501

print(poc_description)

wrong1 = Wrong1_MissingMethod()  # <-- mypy will (rightfully) complain here
wrong1.another_method()

wrong2 = Wrong2_ArgTypeMismatch()
wrong2.do_a_barrel_roll(99)

wrong3 = Wrong3_ReturnTypeMismatch()
wrong3.do_a_barrel_roll("luke")

correct = Correct()
correct.do_a_barrel_roll("luke")

print("If you see this message, it's because at runtime, all was OK :-)")
