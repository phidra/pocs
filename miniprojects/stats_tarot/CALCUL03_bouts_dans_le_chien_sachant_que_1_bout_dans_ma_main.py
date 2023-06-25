#!/usr/bin/env python
# coding: utf-8

from utils import C, A, f, assert_almost_equal, display_percent

"""
AUCUN BOUT DANS LE CHIEN SACHANT QUE J'EN AI UN DANS MA MAIN :
    Arrangements :
        on s'intéresse aux 18 premières cartes (les 15 de la main, et les 3 du chien)
        jeux de 18 cartes avec 1 bouts dans ma main :
            3 possibilités de choisir le bout, 15 possibilités de le positionner dans ma main
            A(14,75) possibilités de choisir les autres cartes de la main (parmi celles sans bout)
            A(3,63) possibilités de choisir les 3 cartes du chien parmi celles restantes, y compris avec les bouts
            ------------------------------------------
            N(total) =  15 * 3 * A(14,75) * A(3,63)
            N(total) =  15 * 3 * (75! / 61!) * (63! / 60!)
            N(total) =  15 * 3 *  75*74*...*62  *  63*62*61
        jeux de 18 cartes avec 1 bout dans ma main, et exactement 0 bout dans le chien :
            3 possibilités de choisir le bout, 15 possibilités de le positionner dans ma main
            A(14,75) possibilités de choisir les autres cartes de la main (parmi celles sans bout)
            A(3,61) possibilités de choisir les 3 cartes du chien parmi celles restantes, sans les 2 bouts restants
            ------------------------------------------
            N(aucun bout) =  15 * 3 * A(14,75) * A(3,61)
            N(aucun bout) =  15 * 3 * (75! / 61!) * (61! / 58!)
            N(aucun bout) =  15 * 3 *  75*74*...*62  *  61*60*59
        P = N(aucun bout) / N(total)
        P = 61*60*59  /  63*62*61
        P = 60*59  /  63*62
    Probabiliste :
        le problème se formule "étant donnés 63 cartes possibles dont 2 bouts, j'en tire 3, quelle probabilité d'avoir aucun bout dans le chien ?"
        P  = (C1 pas bout  ET  C2 pas bout  ET  C3 pas bout)
        P  = 61/63 * 60/62 * 59/61
        P = 61*60*59  /  63*62*61
        P = 60*59  /  63*62
    OK, je retrouve la même valeur avec les 2 calculs.


EXACTEMENT UN BOUT DANS LE CHIEN SACHANT QUE J'EN AI UN DANS MA MAIN :
    Arrangements :
        jeux de 18 cartes avec 1 bouts dans ma main (déjà calculé plus haut) :
            N(total) =  15 * 3 * A(14,75) * A(3,63)
            N(total) =  15 * 3 * (75! / 61!) * (63! / 60!)
            N(total) =  15 * 3 *  75*74*...*62  *  63*62*61
        jeux de 18 cartes avec 1 bout dans ma main, et exactement 1 bout dans le chien :
            3 possibilités de choisir le bout, 15 possibilités de le positionner dans ma main
            A(14,75) possibilités de choisir les autres cartes de la main (parmi celles sans bout)
            2 possibilités de choisir le bout parmi ceux restants
            3 possibilités de positionner le bout choisi dans le chien
            A(2,61) possibilités de choisir les 2 autres cartes du chien parmi celles restantes, sans le bout restant
            ------------------------------------------
            N(1 bout) =   15 * 3 * A(14,75) * 2 * 3 * A(2,61)
            N(1 bout) =  15 * 3 * (75! / 61!) * 6 * (61! / 59!)
            N(1 bout) =  15 * 3 *  75*74*...*62  * 6 *  61*60
        P = N(1 bout) / N(total)
        P =  6 *  61*60    /    63*62*61
        P =  6 * 60  /  63*62
    Probabiliste :
        le problème se formule "étant donnés 63 cartes possibles dont 2 bouts, j'en tire 3, quelle probabilité d'avoir exactement 1 bout dans le chien ?"
        P  = (C1 bout et pas C2 et C3)  OU  (C2 bout et pas C1 et C3)  OU  (C3 bout et pas C1 et C2)    (mutuellement exclusifs)
        P = (2/63 * 61/62 * 60/61) + (61/63 * 2/62 * 60/61) + (61/63 * 60/62 * 2/61)
        P = (2*60*61 / 61*62*63)  +  (2*60*61 / 61*62*63) + (2*60*61 / 61*62*63)
        P = 3 * 2*60*61 / 61*62*63
        P = 6*60 / 62*63
        P = 6.*60 / (62*63)
    OK, je retrouve la même valeur avec les 2 calculs.


EXACTEMENT DEUX BOUTS DANS LE CHIEN SACHANT QUE J'EN AI UN DANS MA MAIN :
    Arrangements :
        jeux de 18 cartes avec 1 bouts dans ma main (déjà calculé plus haut) :
            N(total) =  15 * 3 * A(14,75) * A(3,63)
            N(total) =  15 * 3 * (75! / 61!) * (63! / 60!)
            N(total) =  15 * 3 *  75*74*...*62  *  63*62*61
        jeux de 18 cartes avec 1 bout dans ma main, et exactement 2 bouts dans le chien :
            3 possibilités de choisir le bout, 15 possibilités de le positionner dans ma main
            A(14,75) possibilités de choisir les autres cartes de la main (parmi celles sans bout)
            3*2 = 6 possibilités de positionner les deux bouts restants dans le chien
            61 possibilités de choisir la dernière carte du chien
            ------------------------------------------
            N(2 bouts) =   15 * 3 * A(14,75) * 6 * 61
            N(2 bouts) =  15 * 3 * (75! / 61!) * 6 * 61
            N(2 bouts) =  15 * 3 *  75*74*...*62  * 6 * 61
        P = N(2 bouts) / N(total)
        P =  15 * 3 *  75*74*...*62  * 6 * 61   /   15 * 3 *  75*74*...*62  *  63*62*61
        P =  6 * 61  /  63*62*61
        P =  6 / 63*62
    Probabiliste :
        le problème se formule "étant donnés 63 cartes possibles dont 2 bouts, j'en tire 3, quelle probabilité d'avoir exactement 2 bouts dans le chien ?"
        P = (C1 et C2 bout mais pas C3) OU (C1 et C3 bout mais pas C2) OU (C2 et C3 bout mais pas C1)    (mutuellement exclusifs)
            à noter que les évènements des OU sont mutuellement exlusifs -> je peux sommer les probabilités
            P = (2/63 * 1/62 * 61/61) + (2/63 * 61/62 * 1/61) + (61/63 * 2/62 * 1/61)
            P = (2*61 / 61*62*63)  +  (2*61 / 61*62*63)  +  (2*61 / 61*62*63)
            P = 6 / 62*63
            P = 6. / float(62*63)
    OK, je retrouve la même valeur avec les 2 calculs.



AU MOINS UN BOUT DANS LE CHIEN SACHANT QUE J'EN AI UN DANS MA MAIN :
    Probabiliste :
        P  =  1 - p(C1 pas un bout  ET  C2 pas un bout  ET  C3 pas un bout)
        en utilisant le raisonnement d'exemple dans mes notes de proba, P vaut 1 - (61/63 * 60/62 * 59/61)
        P = 1 - (61*60*59 / (63*62*61))
        P = 1 - (60*59) / (63*62)
        P = float(63*62 - 60*59) / (63*62)
"""

exactement_0_bout = float(60*59) / (63*62)
exactement_1_bout = float(6*60) / (62*63)
exactement_2_bout = float(6) / (62*63)
au_moins_1_bout = float(63*62 - 60*59) / (63*62)

# Vérification :
assert_almost_equal(au_moins_1_bout, (exactement_2_bout + exactement_1_bout))
assert_almost_equal(exactement_0_bout, 1.0 - (exactement_2_bout + exactement_1_bout))


print ""
print "Sachant que j'ai un bout dans ma main, probabilités d'avoir dans le chien :"
print "   - exactement 0 bout = ", display_percent(exactement_0_bout)
print "   - exactement 1 bout = ", display_percent(exactement_1_bout)
print "   - exactement 2 bout = ", display_percent(exactement_2_bout)
print "   - au moins 1 bout   = ", display_percent(au_moins_1_bout)
