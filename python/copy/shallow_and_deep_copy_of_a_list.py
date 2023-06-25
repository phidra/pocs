#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# QUESTION :
#       - supposons une liste l, contenant divers éléments
#       - avec une copie simple (l1 = l),          la liste l est-elle modifiée si j'appelle del(l1[0]) ?
#       - avec une copie explicite (l2 = list(l)), la liste l est-elle modifiée si j'appelle del(l2[0]) ?
#       - avec une shallow-copie l3 de l,          la liste l est-elle modifiée si j'appelle del(l3[0]) ?
#       - avec une deep-copie l4 de l,             la liste l est-elle modifiée si j'appelle del(l4[0]) ?
#
# NOTE : au passage, del(l[0]) ne supprimera pas l'objet, uniquement le fait que la liste y fasse référence.
#        je ne cours donc pas le risque d'invalider un "pointeur" d'une autre liste (et ça explique les résultats).
#
# RÉPONSE :
#   - lorsque la copie référence la même liste, la liste initiale est modifiée (normal, puisqu'on travaille sur le MÊME objet)
#   - lorsque la copie contient une liste DIFFÉRENTE, la liste initiale n'est pas modifiée ET elle pointe vers des objets valides.
#     (en effet, "del" se contente de supprimer le lien entre l'objet pointé et la liste, mais l'objet pointé lui-même ne sera
#     supprimé par le garbage collector que lorsqu'on appellera "del" sur la dernière variable le référençant)
# 
#
########################################################################################################################

import copy


# PREMIER TEST, copie simple :
l = ["a", "b", "c", "d"]
l1 = l
print "id(l)  = ", id(l)
print "id(l1) = ", id(l1)
print "l avant = ", l
del(l1[0])
print "l après = ", l
print ""


# TROISIÈME TEST, copie explicite (création d'une nouvelle liste) :
l = ["a", "b", "c", "d"]
l2 = list(l)
print "id(l)  = ", id(l)
print "id(l2) = ", id(l2)
print "l avant = ", l
del(l2[0])
print "l après = ", l
print ""


# TROISIÈME TEST, shallow-copy (création d'une nouvelle liste) :
l = ["a", "b", "c", "d"]
l3 = copy.copy(l)
print "id(l)  = ", id(l)
print "id(l3) = ", id(l3)
print "l avant = ", l
del(l3[0])
print "l après = ", l
print ""


# QUATRIÈME TEST, deep-copy (création d'une nouvelle liste) :
l = ["a", "b", "c", "d"]
l4 = copy.deepcopy(l)
print "id(l)  = ", id(l)
print "id(l4) = ", id(l4)
print "l avant = ", l
del(l4[0])
print "l après = ", l
print ""

########################################################################################################################
# Fin du fichier.
########################################################################################################################

