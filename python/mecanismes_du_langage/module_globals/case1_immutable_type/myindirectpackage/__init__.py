#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
This is the content of myindirectpackage.

It INDIRECTLY defines a global variable, and a handle to modify it.
"""

import sys ; sys.dont_write_bytecode = True

from . import hiddenmodule

myvariable3 = hiddenmodule.myvariable3
print "INSIDE MYINDIRECTMODULE - init : variable id         = ", id(myvariable3)
setter3 = hiddenmodule.setter3
