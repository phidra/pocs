#!/usr/bin/env python
# coding: utf-8

from utils import C, A, f, assert_almost_equal, display_percent

"""
AUCUN BOUT AU CHIEN :
    Arrangements :
        P = A(3,75)  /  A(3,78)
        P = 75!/72!  /  (78!/75!)
        P = float(75*74*73) / (78*77*76)
    Calcul probabiliste :
        P = p(aucun bout)
        P = p(C1 pas un bout  ET  C2 pas un bout  ET  C3 pas un bout)
        P = 75/78 * 74/77 * 73/76
        P = float(75*74*73) / (78*77*76)
    OK, je retrouve le même résultat avec les 2 calculs

EXACTEMENT UN BOUT AU CHIEN :
    Calcul par arrangements (sur des mains de 3 cartes) :
        3 choix pour le bout
        3 positions possibles pour le bout
        A(2,75) pour les autres cartes
        N = 3 * 3 * 75! / 73!
        P = (3 * 3 * 75! / 73!) / (78! / 75!)
        P = 3. * 3*75*74  / (78*77*76)
    Calcul probabiliste :
        P = p(1 bout au chien)
        P = p(C1 bout, C2 et C3 pas bout  OU  C2 bout, C1 et C3 pas bout  OU  C3 bout, C1 et C2 pas bout)  mutuellement exclusifs
        P = 3  *  (3/78 * 75/77 * 74/76)
        P = 3. * 3*75*74 / (78*77*76)
    OK, je retrouve le même résultat avec les 2 calculs

EXACTEMENT DEUX BOUTS AU CHIEN :
    Calcul par arrangements (sur des mains de 3 cartes) :
        3 choix pour 2 bouts
        3 * 2 positions possibles pour les bouts
        75 possibilités pour l'autre carte
        N = 3 * 3*2 * 75
        P = 3 * 3*2 * 75  /  A(3,78)
        P = 3. * 6  * 75  / (78*77*76)
    Calcul probabiliste :
        P = p(2 bouts au chien)
        P = p(C1 et C2 bout, C3 pas bout  OU  C1 et C3 bout, C2 pas bout  OU  C2 et C3 bout, C1 pas bout)  mutuellement exclusifs
        P = 3  *  (3/78 * 2/77 * 75/76)
        P = 3. * 3*2*75 / (78*77*76)
    OK, je retrouve le même résultat avec les 2 calculs

EXACTEMENT TROIS BOUTS AU CHIEN :
    Calcul par arrangements (sur des mains de 3 cartes) :
        permutations(3) pour les emplacements des 3 bouts
        N = 3!
        N = 6
        P = 6  /  A(3,78)
        P = 6. / (78*77*76)
    Calcul probabiliste :
        P = p(3 bouts au chien)
        P = 3/78 * 2/77 * 1/76
        P = 6. / (78*77*76)
    OK, je retrouve le même résultat avec les 2 calculs

AU MOINS UN BOUT :
    Calcul probabiliste :
        P = p(au moins un bout)
        P = 1 - p(aucun bout)      (déjà calculé plus haut)
        P = 1. - (float(75*74*73) / (78*77*76))

AU MOINS DEUX BOUTS :
    Calcul probabiliste :
        P = p(au moins deux bouts)
        P = 1 - p(au plus 1 bout)
        P = 1 - [ p(aucun bout) + p(exactement 1 bout) ]
        P = 1 - P1 - P2
        On a vu plus haut :
            P1 = (75/78 * 74/77 * 73/76)
            P2 = 3. * 3*75*74 / (78*77*76)
        P = 1. - (float(75*74*73) / (78*77*76)) - (3. * 3*75*74 / (78*77*76))
"""

exactement_0_bout = float(75*74*73) / (78*77*76)
exactement_1_bout = 3. * 3*75*74 / (78*77*76)
exactement_2_bout = 3. * 3*2*75 / (78*77*76)
exactement_3_bout = 6. / (78*77*76)

au_moins_1_bout = 1. - (float(75*74*73) / (78*77*76))
au_moins_2_bout = 1. - (float(75*74*73) / (78*77*76)) - (3. * 3*75*74 / (78*77*76))

# Vérification :
assert_almost_equal(au_moins_1_bout, (exactement_3_bout + exactement_2_bout + exactement_1_bout))
assert_almost_equal(au_moins_2_bout, (exactement_3_bout + exactement_2_bout))
assert_almost_equal(exactement_0_bout, 1.0 - (exactement_3_bout + exactement_2_bout + exactement_1_bout))


print ""
print "Probabilités d'avoir dans le chien :"
print "   - exactement 0 bout = ", display_percent(exactement_0_bout)
print "   - exactement 1 bout = ", display_percent(exactement_1_bout)
print "   - exactement 2 bout = ", display_percent(exactement_2_bout)
print "   - exactement 3 bout = ", display_percent(exactement_3_bout)
print "   - au moins 1 bout   = ", display_percent(au_moins_1_bout)
print "   - au moins 2 bout   = ", display_percent(au_moins_2_bout)
