#!/usr/bin/env python
# coding: utf-8

import random

N_ITERATIONS = 3000000
total = N_ITERATIONS
match = 0
N_CARTES = 78

state = range(N_CARTES)  # initialement, les cartes sont triées


# ici, (mais c'est une POC), on calcule la probabilité d'avoir au moins 2 bouts dans la main

for _ in range(N_ITERATIONS):
    random.shuffle(state)
    hand = state[:15]

    # Au moins 1 bout dans la main :
    if (0 in hand) or (1 in hand) or (2 in hand):
        match += 1


    # # Au moins 2 bouts dans la main :
    # if (
    #         (0 in hand and 1 in hand) or
    #         (1 in hand and 2 in hand) or
    #         (0 in hand and 2 in hand) or
    #         (0 in hand and 1 in hand and 2 in hand)
    #     ):
    #     match += 1


print "CALCULATED = ", float(match) / total
    

