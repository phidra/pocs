#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# NE FONCTIONNE QU'À PARTIR DE PYTHON 2.7 !
#
# Les OrderedDicts sont des dict qui se souviennent de l'ordre d'insertion des éléments dans le dict.
# (ce sont donc des conteneurs différents des std::map du C++)
# On peut les utiliser là où on utilise des dictionnaires normaux.
# 
# Quelques précisions sur l'ordre :
#     - si on modifie la valeur d'une clé déjà insérée, l'ordre est inchangé.
#     - par contre, si on supprime une paire clé-valeur, et qu'on la réajoute, elle est ajoutée à la fin.
# 
# Ils disposent d'une méthode supplémentaire par rapport au dict :
#     - popitem : supprime et renvoie une paire clé/valeur (soit au début, soit à la fin du dict).
# 
########################################################################################################################

import collections

oc = collections.OrderedDict()
d  = {}

l = ["Luke", "Leia", "Obi-wan", "Anakin", "Yoda", "Palpatine", "Dooku", "Qui-gon", "Jar-jar", "Padme", "Shmii"]


for index,value in enumerate(l):
    oc[value] = index
    d[value] = index


########################################################################################################################
# L'ordre d'insertion est mémorisé :
########################################################################################################################

print "Dictionnaire classique :"
for key,value in d.iteritems():
    print "\t", key, value

print "OrderedDict :"
for key,value in oc.iteritems():
    print "\t", key, value

########################################################################################################################
# On peut récupérer le premier et le dernier :
########################################################################################################################

first_key, first_value = oc.popitem(last=False)
last_key, last_value = oc.popitem(last=True)

print ""
print "Premier element = [{k} : {v}]".format(k=first_key, v=first_value)
print "Dernier element = [{k} : {v}]".format(k=last_key, v=last_value)

########################################################################################################################
# Fin du fichier.
########################################################################################################################
