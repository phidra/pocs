#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# MA QUESTION :
#       - supposons une fonction f, qui prend une liste l en argument
#       - si dans le corps de f, on appelle del(l[0]), que se passe-t-il ?
# MES PROPOSITIONS :
#       - proposition 1 = une copie locale de l est modifiée, mais la liste à l'extérieur ne l'est pas
#       - proposition 2 = la liste à l'extérieur de f est modifiée (son premier élément est supprimé)
#       - proposition 3 = levée d'une exception interdisant de modifier l'argument
# CE QUI SE PASSE EFFECTIVEMENT D'APRÈS LA POC :
#       - proposition 2 = la liste à l'extérieur de f est modifiée (son premier élément est supprimé)
# 
# CONTEXTE = Je voulais dans une fonction itérer de façon aléatoire sur des éléments de la liste, sans doublon.
# Une façon de faire ça était de choisir un élément au hasard, puis de le supprimer de la liste...
# Mais je ne voulais pas modifier la liste initiale → dois-je faire une copie ? Réponse, oui. 
#
# EDIT : cf. aussi les pypocs sur les scopes et les closures.
#
########################################################################################################################


# PREMIER CAS = tester l'énoncé de la poc :
def f(l):
    print "<inside f>  l = ", l
    del(l[0])
    print "<inside f>  l = ", l

outside_l = ["a", "b", "c", "d"]
print "<outside f> l = ", outside_l
f(outside_l)
print "<outside f> l = ", outside_l

########################################################################################################################

# DEUXIÈME CAS = idem, mais en travaillant dans f avec une COPIE de la liste :
import copy
print ""
print "===================================================================================================="
print ""
def f2(l):
    l_copy = list(l)
    print "<inside f2>  l_copy = ", l_copy
    del(l_copy[0])
    print "<inside f2>  l_copy = ", l_copy

outside_l = ["a", "b", "c", "d"]
print "<outside f2> l = ", outside_l
f2(outside_l)
print "<outside f2> l = ", outside_l

########################################################################################################################
# Fin du fichier.
########################################################################################################################

