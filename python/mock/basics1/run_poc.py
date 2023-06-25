#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
#
# Cette POC montre une utilisation simple du module mock (unittest.mock en python3).
#
#     cf. https://docs.python.org/3/library/unittest.mock.html
#
# Le principe de la POC est le suivant :
#     on a une fonction de traitement, "process", qu'on souhaite tester unitairement
#     cette fonction de traitement utilise "randint" pour récupérer un entier aléatoire
#     l'objectif de la POC est de remplacer randint par un mock pour que le test unitaire soit répétable
#
# Aller voir "run_test.py" pour voir comment la fonction est mockée.
#
########################################################################################################################

import sys ; sys.dont_write_bytecode = True

def randint():
    """ On choisit un nombre au hasard dans la liste [0, 0, 0, 0, 0, 1, 2, 3, 4, 5] """
    sequence = 5*[0] + range(1,6)
    import random
    return random.choice(sequence)


def process():
    """
    Fonction de traitement :
        - récupère un entier via randint
        - s'il est nul, on renvoie 42
        - sinon, on renvoie le double de l'entier
    """
    myint = randint()
    if not myint:
        return 42
    else:
        return 2 * myint


if __name__ == "__main__":
    for i in range(6):
        print "PROCESSED = ", process()
