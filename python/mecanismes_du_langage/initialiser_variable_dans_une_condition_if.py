#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# POC sur le fait d'initialiser une variable dans une condition.
#
# CONCLUSIONS DE LA POC :
#       ça n'a pas l'air possible, puisqu'on se prend une "SyntaxError: invalid syntax".
#
########################################################################################################################

valeur_copiee = True

if (a = valeur_copiee):
    print u"Condition évaluée à vrai, a = ", a
else:
    print u"Condition évaluée à faux"


########################################################################################################################
# Fin du fichier.
########################################################################################################################

