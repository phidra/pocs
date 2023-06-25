#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
This is the hidden module of myindirectpackage.
"""

import sys ; sys.dont_write_bytecode = True

myvariable3 = [44]
print "INSIDE MYINDIRECTMODULE - hiddenmodule : variable id = ", id(myvariable3)

def setter3(newvalue):
    global myvariable3
    myvariable3.append(newvalue)
    del myvariable3[0]
