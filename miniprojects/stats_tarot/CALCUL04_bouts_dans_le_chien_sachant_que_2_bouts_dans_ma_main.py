#!/usr/bin/env python
# coding: utf-8

from utils import C, A, f, assert_almost_equal, display_percent

"""
AUCUN BOUT DANS LE CHIEN SACHANT QUE J'EN AI DEUX DANS MA MAIN :
    Arrangements :
        on s'intéresse aux 18 premières cartes (les 15 de la main, et les 3 du chien)
        jeux de 18 cartes avec 2 bouts dans ma main :
            15*14 possibilités de placer 2 bouts dans ma main
            C(2,3) = 3 possibilités de choisir 2 bouts non-ordonnés
            A(13,75) possibilités de choisir les 13 autres cartes de la main, parmi celles restantes SANS BOUT
            A(3,63) possibilités de choisir les 3 cartes du chien parmi celles restantes (y compris le bout)
            ----------------------------------------------------------------
            N(total) =  15 * 14 * 3 * (75! / 62!) * (63! / 60!)
            N(total) =  15 * 14 * 3 * 75*74*...*63 * 63*62*61
        jeux de 18 cartes avec 2 bouts dans ma main et 0 bout dans le chien :
            15*14 possibilités de placer 2 bouts dans ma main
            C(2,3) = 3 possibilités de choisir 2 bouts non-ordonnés
            A(13,75) possibilités de choisir les 13 cartes restantes pour la main, parmi celles sans bout
            A(3,62) possibilités de choisir les 2 cartes restantes pour le chien, parmi celles sans bout
            ----------------------------------------------------------------
            N(1 bout au chien) = 15 * 14 * 3 * (75! / 62!) * (62! / 59!)
            N(1 bout au chien) = 15 * 14 * 3 * 75*74*...*63 * 62*61*60
        P = N(1 bout au chien) / N(total)
        P = 75*74*...*63 * 62*61*60   /   75*74*...*63 * 63*62*61
        P = 60 / 63
    Probabiliste :
        P = P(B1 pas bout  ET  B2 pas bout  ET  B3 pas bout)
        P = 62/63  *  61/62  *  60/61
        P = 60 / 63
    OK, je retrouve la même valeur avec les 2 calculs.

EXACTEMENT UN BOUT DANS LE CHIEN SACHANT QUE J'EN AI DEUX DANS MA MAIN :
    Arrangements :
        on s'intéresse aux 18 premières cartes (les 15 de la main, et les 3 du chien)
        jeux de 18 cartes avec 2 bouts dans ma main :
            15*14 possibilités de placer 2 bouts dans ma main
            C(2,3) = 3 possibilités de choisir 2 bouts non-ordonnés
            A(13,75) possibilités de choisir les 13 autres cartes de la main, parmi celles restantes SANS BOUT
            A(3,63) possibilités de choisir les 3 cartes du chien parmi celles restantes (y compris le bout)
            ----------------------------------------------------------------
            N(total) =  15 * 14 * 3 * (75! / 62!) * (63! / 60!)
        jeux de 18 cartes avec 2 bouts dans ma main et 1 bout dans le chien :
            15*14 possibilités de placer 2 bouts dans ma main
            C(2,3) = 3 possibilités de choisir 2 bouts non-ordonnés
            A(13,75) possibilités de choisir les 13 cartes restantes pour la main, parmi celles sans bout
            3 possibilités de placer le 3ième bout dans le chien
            A(2,62) possibilités de choisir les 2 cartes restantes pour le chien
            ----------------------------------------------------------------
            N(1 bout au chien) = 15 * 14 * 3 * (75! / 62!) * 3 * (62! / 60!)
        P = N(1 bout au chien) / N(total) = 3 / 63
    Probabiliste :
        En reformulant le problème, on suppose que j'ai une carte (le bout) à distribuer parmi 63 emplacements.
        Quelle est la probabilité que je la distribue dans le chien, qui représente 3 emplacements parmi 63 ?
        Formulé comme ça, le résultat est immédiat.
        P = 3 / 63
    OK, je retrouve la même valeur avec les 2 calculs.
"""

exactement_0_bout = 60. / 63
exactement_1_bout = 3.  / 63

# Vérification :
assert_almost_equal(exactement_0_bout, 1.0 - exactement_1_bout)


print ""
print "Sachant que j'ai deux bouts dans ma main, probabilités d'avoir dans le chien :"
print "   - exactement 0 bout = ", display_percent(exactement_0_bout)
print "   - exactement 1 bout = ", display_percent(exactement_1_bout)
