#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Ce module externe expose une fonction permettant de récupérer un entier, selon une distribution un peu particulière.
########################################################################################################################

import sys ; sys.dont_write_bytecode = True

import random

def get():
    """
    Renvoie un entier entre 0 et 9 inclus.
    On a une chance sur deux d'avoir 0, et une chance sur deux d'avoir un chiffre entre 1 et 9 inclus.
    """
    # On construit la liste [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    sequence = 9*[0] + range(1,10)
    return random.choice(sequence)
