#!/usr/bin/env python3

from functools import singledispatch


# cf. https://www.python.org/dev/peps/pep-0443/


@singledispatch
def do_speak(arg, verbose=False):
    raise NotImplementedError


@do_speak.register(str)
def _do_speak_str(arg, verbose=False):
    if verbose:
        print("Let me just say,", end=" ")
    print(arg)


@do_speak.register(int)
def _do_speak_int(arg, verbose=False):
    if verbose:
        print("Strength in numbers, eh?", end=" ")
    print(arg)


@do_speak.register(list)
def _do_speak_list(arg, verbose=False):
    if verbose:
        print("Enumerate this:")
    for elem in arg:
        do_speak(elem)


def print_an_ode_to_None(arg, verbose=False):
    print("None is good, eat some None, live by the None")


# registering an already existing function (versus decorating a function while defining it) :
do_speak.register(type(None), print_an_ode_to_None)


print("")

do_speak("coucou")
print("===")
do_speak(42)
print("===")
do_speak([1, 2, 99])
print("===")
do_speak(False)
print("===")
do_speak(None)
print("===")

# non-implemented types are not detected, this fallbacks on the "initial" implementation.
# mypy behaviour : mypy does NOT complain that `do_speak` is not implemented for floats here :
try:
    do_speak(3.1415)
except NotImplementedError:
    print("WOW, we just caught a call with a non-implemented type...")


# sadly, it IS possible to register a function with mismatching interface (nb arguments) :
# mypy behaviour : mypy does NOT complain that the specialisation has not the proper number of args here :
@do_speak.register(bytes)
def _do_speak_bytes(arg, coucou, pouet, youpi):
    print("...called with bytes...")


# in this case, the specific call must follow the mismatching interface :
do_speak(b"pouet", 42, 43, 44)

print("END OF POC")
print("")
