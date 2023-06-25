#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Module externe contenant des fonctions, pour tester leur appelabilité par clize.
########################################################################################################################

import clize
 
@clize.clize()
def troisieme_sous_commande(arg1, arg2, option1 = False, option2 = "pouet", *fin_des_args):
    u""" Aide de la fonction du module externe. """
    print "Appel de la sous-commande n°3"


########################################################################################################################
# Fin du fichier.
########################################################################################################################

