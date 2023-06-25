#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# MA QUESTION :
#       - supposons une fonction f, qui prend un entier i en argument.
#       - si dans le corps de f, on appelle i += 1, que se passe-t-il ?
# CE QUI SE PASSE EFFECTIVEMENT D'APRÈS LA POC :
#       - en interne à la fonction, l'entier est modifié (plus exactement, "i" pointe vers un objet d'id différent)
#       - l'objet pointé par la variable à l'extérieur de la fonction n'est pas modifié (il conserve le même id)
#
# EDIT : cf. aussi les pypocs sur les scopes et les closures.
#
########################################################################################################################


# Tester l'énoncé de la poc :
def f(i):
    print "< inside f>  i [id = {id}] = ".format(id = id(i)), i
    i += 1
    print "< inside f>  i [id = {id}] = ".format(id = id(i)), i


outside_i = 42
print "<outside f>  i [id = {id}] = ".format(id = id(outside_i)), outside_i
f(outside_i)
print "<outside f>  i [id = {id}] = ".format(id = id(outside_i)), outside_i

########################################################################################################################
# Fin du fichier.
########################################################################################################################

