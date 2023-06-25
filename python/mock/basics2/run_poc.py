#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
#
# Cette POC montre une utilisation un peu plus évoluée du module mock (unittest.mock en python3).
# Par rapport à la POC basics1, on ajoute un niveau d'indirection, puisque la méthode mockée est dans un sous-module.
#
#     cf. https://docs.python.org/3/library/unittest.mock.html
#
# Le principe de la POC reste le même que pour basics1 :
#     on a une fonction de traitement, "process", qu'on souhaite tester unitairement
#     cette fonction de traitement utilise "randint" pour récupérer un entier aléatoire
#     l'objectif de la POC est de remplacer randint par un mock pour que le test unitaire soit répétable
#
# Aller voir "run_test.py" pour voir comment la fonction est mockée.
#
# OBJECTIF = montrer qu'on peut mocker une fonction d'un sous-module même pas importé dans le test.
# (en effet, on se contente de remplacer des variables dans des espaces de noms, cf. explications dans le test)
#
########################################################################################################################

import sys ; sys.dont_write_bytecode = True

import intgetter

def process():
    """
    Fonction de traitement :
        - récupère un entier via intgetter
        - s'il est nul, on renvoie 42
        - sinon, on renvoie le double de l'entier
    """
    myint = intgetter.randint()
    if not myint:
        return 42
    else:
        return 2 * myint


if __name__ == "__main__":
    for i in range(5):
        print "PROCESSED = ", process()
