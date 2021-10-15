#!/usr/bin/env python3

from mysuperpackage.greeter1 import greet  # name
from mysuperpackage import greeter1  # module
from mysuperpackage import sub  # subpackage

print(
    """
    This POC shows that the synxax 'from X import Y' may be used to import 3 different things :
        1. Y is the name of an object in the module X
        2. Y is a module in the package X
        3. Y is a subpackage in the package X (in which case names are pulled from its __init__.py)

    Said otherwise : if we are interested in the module containing the imported code, it could be :
        1. X.py
        2. X/Y.py
        3. X/Y/__init__.py

    See:
        https://docs.python.org/3/tutorial/modules.html#packages
"""
)

greet("pouet1")
greeter1.greet("pouet2")
sub.greet("pouet3")
