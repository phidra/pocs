#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Module de la POC, qui essaye de récupérer les arguments passés à main, directement dans le sys.argv
#
########################################################################################################################


print "coucou"

import sys

def f():
    print u"Dans le MODULE, il y a {} éléments dans le sys.argv, qui sont :".format(len(sys.argv)).encode("utf-8")
    for element in sys.argv:
        print "\t* {}".format(element)


########################################################################################################################
# Fin du fichier.
########################################################################################################################
