#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Ce module externe expose une fonction permettant de récupérer un entier aléatoirement.
########################################################################################################################

import sys ; sys.dont_write_bytecode = True
import random

def randint():
    """ On choisit un nombre au hasard dans la liste [0, 0, 0, 0, 0, 1, 2, 3, 4, 5] """
    sequence = 5*[0] + range(1,6)
    return random.choice(sequence)
