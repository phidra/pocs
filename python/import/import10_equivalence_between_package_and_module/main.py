#!/usr/bin/env python3

import greeter1
import greeter2

print(
    """
    This POC shows that a module file is equivalent to a package's init file :

    Content of the POC :
    - a module 'greeter1' defines a 'greet' function
    - a package 'greeter2' contains an __init__ file that also defines a 'greet' function
    - the main uses both greeter1.greet() and greeter2.greet() in the same way

    CONCLUSION : for a direct use, a module and a package's __init__ are equivalent.
"""
)

# they behave the same, although one is a regular module, the other is a package :
greeter1.greet()
greeter2.greet()
