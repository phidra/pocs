#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
This is the content of mydirectpackage.

It DIRECTLY defines a global variable, and a handle to modify it.
"""

import sys ; sys.dont_write_bytecode = True

myvariable2 = 43
print "INSIDE MYDIRECTMODULE : variable id = ", id(myvariable2)

def setter2(newvalue):
    global myvariable2
    myvariable2 = newvalue


