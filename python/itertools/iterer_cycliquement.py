#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# POC sur le fait d'itérer cycliquement (itération infinie, qui boucle au début de l'itérateur une fois à la fin).
# On pourrait boucler infiniment...
#
########################################################################################################################

import itertools


iterable = ("a", "b", "c")

print ""
print "ITÉRABLE À PARCOURIR = ", iterable
print ""


# Récupération de l'itérateur :
my_iterator = itertools.cycle(iterable)
n_iterations = 15


print "On va itérer {} fois :".format(n_iterations)
for i in range(n_iterations):
    print "\t[{i:02}] {value}".format(i=i, value=my_iterator.next())


########################################################################################################################
# Fin du fichier.
################################################################################

