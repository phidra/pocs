#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# QUESTION : comment coder un pile-ou-face biaisé (p.ex. qui a 70% de chances de tomber sur pile, au lieu de 50%) ?
#
# RÉPONSE :
# random.random() retourne un float entre 0 (inclus) et 1 (exclus), avec une probabilité uniforme.
# conséquemment, pour 0 <= p <= 1, l'évènement (random.random() < p) se produit avec une probabilité p.
#
########################################################################################################################

import random

def biased_coin_flip_is_true(probability_to_get_true = 0.5):
    u""" Simule un lancer de pièce, et renvoie True si la pièce est tombée sur face.
         La probabilité que la pièce tombe sur face est passée en paramètre (par défaut, 1 chance sur 2)."""

    result = random.random() # résultat compris uniformément entre 0 et 1
    return result <= probability_to_get_true # sera vrai avec une probabilité "probability_to_get_true"

counter = 0
nb_of_launch = 10000
expected_heads_probability = 0.7

for i in range(nb_of_launch):
    if biased_coin_flip_is_true(expected_heads_probability):
        counter += 1

print u"Probabilité demandée = {expected}".format(expected = expected_heads_probability).encode("utf-8")
print u"Probabilité obtenue  = {proba}".format(proba = float(counter) / nb_of_launch).encode("utf-8")

########################################################################################################################
# Fin du fichier.
########################################################################################################################

