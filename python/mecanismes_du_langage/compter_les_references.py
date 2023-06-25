#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# POC sur la façon de compter les références.
#
########################################################################################################################

import sys

VARIABLE = "coucou"

print "État initial :"
print "\tgetrefcount(VARIABLE) = ", sys.getrefcount(VARIABLE)
print ""

# Ajout de 10 références :
mylist = []
for i in range(10):
    mylist.append(VARIABLE)

print "Après ajout de 10 références :"
print "\tgetrefcount(VARIABLE) = ", sys.getrefcount(VARIABLE)

########################################################################################################################
# Fin du fichier.
########################################################################################################################


