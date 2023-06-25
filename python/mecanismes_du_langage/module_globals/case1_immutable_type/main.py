#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys ; sys.dont_write_bytecode = True

import mymodule
print "(mymodule) VARIABLE ID        = ", id(mymodule.myvariable1)
print "(mymodule) INITIAL STATE      = ", id(mymodule.myvariable1), ":", mymodule.myvariable1
mymodule.setter1(11)
print "(mymodule) FINAL STATE        = ", id(mymodule.myvariable1), ":", mymodule.myvariable1
print ""


import mydirectpackage
print "(mydirectpackage) VARIABLE ID       = ", id(mydirectpackage.myvariable2)
print "(mydirectpackage) INITIAL STATE     = ", id(mydirectpackage.myvariable2), ":", mydirectpackage.myvariable2
mydirectpackage.setter2(22)
print "(mydirectpackage) FINAL STATE       = ", id(mydirectpackage.myvariable2), ":", mydirectpackage.myvariable2
print ""


import myindirectpackage
import myindirectpackage.hiddenmodule
print "(myindirectpackage.hiddenmodule) VARIABLE ID         = ", id(myindirectpackage.hiddenmodule.myvariable3)
print ""

print "(myindirectpackage) VARIABLE ID                      = ", id(myindirectpackage.myvariable3)
print "(myindirectpackage) INITIAL STATE                    = ", id(myindirectpackage.myvariable3), ":", myindirectpackage.myvariable3, "   <---"
print "(myindirectpackage.hiddenmodule) VARIABLE ID         = ", id(myindirectpackage.hiddenmodule.myvariable3)

print ""
print "== call of the setter3 =="
myindirectpackage.setter3(33)
print "(myindirectpackage.hiddenmodule) VARIABLE ID         = ", id(myindirectpackage.hiddenmodule.myvariable3)
print "(myindirectpackage) FINAL STATE BEFORE RELOAD        = ", id(myindirectpackage.myvariable3), ":", myindirectpackage.myvariable3, "   <---"
reload(myindirectpackage)
print "(myindirectpackage.hiddenmodule) VARIABLE ID         = ", id(myindirectpackage.hiddenmodule.myvariable3)
print "(myindirectpackage) FINAL STATE AFTER  RELOAD        = ", id(myindirectpackage.myvariable3), ":", myindirectpackage.myvariable3, "   <---"
