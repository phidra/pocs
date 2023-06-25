#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# POC sur le fait d'itérer à la main :
#   - on récupère un itérateur avec la méthode "__iter__()" d'un itérable
#   - on peut appeler "next()" sur cet itérateur autant de fois qu'on veut
#   - une fois arrivé à la fin de l'itérable, next() renvoie une exception "StopIteration"
#
########################################################################################################################



iterable = [1, 42, 3.14, "coucou pouet"]

print ""
print "ITÉRABLE À PARCOURIR = ", iterable
print ""


# Récupération de l'itérateur :
my_iterator = iterable.__iter__()
index = 0


# On itère en boucle infinie :
while True:
    index += 1
    value = my_iterator.next()
    print "\tAppel de next n°{index:02} = {value}".format(index = index, value = value)


########################################################################################################################
# Fin du fichier.
################################################################################

