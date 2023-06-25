#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
This is the module.

It defines a global variable, and a handle to modify it.
"""

import sys ; sys.dont_write_bytecode = True

myvariable1 = 42
print "INSIDE MYMODULE : variable id = ", id(myvariable1)

def setter1(newvalue):
    global myvariable1
    myvariable1 = newvalue

