#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
# POC = implémentation de l'exemple donné ici :
#
#    https://docs.python.org/3/library/unittest.mock.html#where-to-patch
#
# OBJECTIF = montrer qu'il faut mocker le nom de variable utilisé par le module
#            (et qui peut être différent de la définition de la variable)
#
#
########################################################################################################################

"""
Ce module run_poc.py est un main qui appelle N fois les fonctions bX.some_function.

cf. les docstrings des modules a, b1 et b2 pour comprendre ce qu'ils font.
"""

import sys ; sys.dont_write_bytecode = True

import b1
import b2

if __name__ == "__main__":
    for i in range(6):
        print "[b1]  CALL({}) = {}".format(i, b1.some_function(i))
        print "[b2]  CALL({}) = {}".format(i, b2.some_function(i))
        print ""
