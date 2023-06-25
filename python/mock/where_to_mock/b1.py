#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# POC = implémentation de l'exemple donné ici :
#
#    https://docs.python.org/3/library/unittest.mock.html#where-to-patch
#
########################################################################################################################

"""
Les deux modules b1 et b2 définissent some_function, qui instancie N fois la classe définit dans a.
La différence est que b1 utilise ligne d'import suivant :

    from a import SomeClass

Tandis que b2 utilise la ligne d'import suivant :

    import a

"""

import sys ; sys.dont_write_bytecode = True

from a import SomeClass

def some_function(N):
    for i in range(N):
        SomeClass()
    return N
