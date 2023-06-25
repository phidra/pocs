#!/usr/bin/env python
# coding: utf-8

from utils import C, A, f, assert_almost_equal, display_percent

"""
AUCUN BOUT DANS MA MAIN :
    Arrangements :
        P = A(15,75) / A(15,78)
        P = 75!/60!  /  78!/63!
        P = 75!/60!  *  63!/78!
        P = 63!/60!  *  75!/78!
        P = float(63*62*60) /  (78*77*76)
    Probabiliste :
        P = p(aucun bout)
        P = p(C1 pas un bout  ET  C2 pas un bout  ET  ...  ET C15 pas un bout)
        P = 75/78 * 74/77 * 73/76 * ... * 61/64    (répété 15 fois, car on tire successivement 15 cartes)
        P = (75 * 74 * ... * 61)  /  (78 * 77 * ... * 64)
        P = (75!/60!) / (78!/63!)
        P = (75!/60!) * (63!/78!)
        P = (63!/60!) * (75!/78!)
        P = 63*62*61  * (1 / (78*77*76))
        P = 63*62*61  /  78*77*76
        P = float(63*62*61)  /  (78*77*76)
        P ~ 0.5219911667280088
    OK, je retrouve les mêmes valeurs

EXACTEMENT UN BOUT DANS MA MAIN :
    Combinaisons :
        P = 3 * C(14,75) / C(15,78)
        P = 3 * (75! / 61!*14!) / (78! / 63!*15!)
        P = 3 * 63*62*15 / 78*77*76
    Arrangements :
        15 possibilités de positionner un bout donné dans la main.
        3 possibilités de choisir le bout
        A(14,75) pour les autres cartes de la main (choisies parmi celles sans bout)
        P = 3 * 15 * (75!/61!)  /  (78!/63!)
        P = 3 * 15*63*62 / 78*77*76
    Probabiliste :
        P = p(B1 mais pas B2 et B3  OU  B2 mais pas B1 et B3  OU  B3 mais pas B1 et B2)  (mutuellement exclusifs)
        P = 15/78*63/77*62/76 + 63/78*15/77*62/76 + 63/78*62/77*15/76
        P = 3 * 15*62*63 / 78*77*76
    OK, je retrouve la même valeur avec les 3 calculs


EXACTEMENT DEUX BOUTS DANS MA MAIN :
    Combinaisons :
        P = C(2,3) * C(13,75) / C(15,78)
        P = 3 * (75! / 62!*13) / (78! / 63!*15!)
        P = 3 * 15*14*63 / 78*77*76
    Arrangements :
        Il y a 15*14 possibilités de positionner 2 bouts donnés dans la main.
        Il y a 3 possibilités de choisir deux bouts sans tenir compte de l'ordre (car l'ordre est déjà pris en compte)
        Pour les autres cartes de la main, il y a A(13,75) possibilités
        P = 3 * 15 * 14 * A(13,75) / A(15,78)
        P = 3 * 15 * 14 * (75!/62!) / (78!/63!)
        P = 3 * 15*14*63 / 78*77*76
    Probabiliste :
        P = p(B1 et B2 mais pas B3  OU  B1 et B3 mais pas B2  OU  B2 et B3 mais pas B1)  (mutuellement exclusifs)
        P = 15/78 * 14/77 * 63/76   +  15/78 * 63/77 * 14/76  +  63/78 * 15/77 * 14/76
        P = 3 * 14*15*63 / 78*77*76
    OK, je retrouve la même valeur avec les 3 calculs.


EXACTEMENT TROIS BOUTS DANS SA MAIN :
    Combinaisons :
        P = C(12,75) / C(15,78)
        P = (75! / 63!*12!)   /  (78! / 63!*15!)
        P = 15*14*13 / 78*77*76
    Arrangements :
        Il y a 15 * 14 * 13 possibilités de positionner 3 bouts dans la main (permutations de 15 cartes avec 12 répétitions)
        Pour les autres cartes de la main, il y a A(12,75) possibilités.
        P = 15*14*13 * (75! / 63!) / (78! / 63!)
        P = 15*14*13 / 78*77*76
    Probabiliste :
        P = p(B1 dans main  ET  B2 dans main  ET B3 dans main)
        P = 15/78 * 14/77 * 13/76
        P = 15*14*13 / 78*77*76
    OK, je retrouve la même valeur avec les 3 calculs.


AU MOINS DEUX BOUTS DANS MA MAIN :
    Combinaisons :
        CALCUL INCORRECT :
            3 possibilités de choisir 2 bouts
            P = 3 * C(13,76) / C(15,78)
            P = 3 * (76! / 63!*13!)  /  (78! / 63!*15!)
            P = 3 * 15*14 / 78*77
            P ~ 0.1048951048951049
        POURQUOI CE CALCUL EST INCORRECT :
            cf. explication dans le calcul avec les arrangements.
        CALCUL CORRECT :
            Je ne le fais pas, mais il faut splitter les cas pour le choix des 13 cartes restantes, cf. plus bas...
    Arrangements :
        CALCUL INCORRECT :
            3 possibilités de choisir 2 bouts
            15*14 possibilités de les positionner
            A(13,76) pour les autres cartes de la main
            P = 3 * 15*14 * (76!/63!)   /  (78!/63!)
            P = 3 * 15*14 / 78*77
            P ~ 0.1048951048951049
        POURQUOI CE CALCUL EST INCORRECT :
            Je supposais que le fait de choisir 2 bouts, puis les 13 cartes restantes parmi les 76 (donc y compris le bout)...
            ... me permettait de couvrir les 2 cas : avec le 3ième bout dans la main, ou sans.
            C'est le cas, mais ça compte TROIS FOIS chaque main à 3 bouts, au lieu de ne la compter qu'une seule fois.
            En effet, on tire d'abord 2 bouts, on les place, puis on place les cartes restantes, parmi lesquelles il peut y avoir un bout
            Du coup, les 3 situations VUES COMME DIFFÉRENTES suivantes conduisent en fait aux mêmes mains :
                    si le 1 et le 21 sont tirés au début, et placés en position 1 et 2, et que l'excuse est tirée après, et placée en 3
                    si le 1 et l'excuse sont tirés au début, et placés en position 1 et 3, et que le 21 et tiré après, et placé en 2
                    si le 21 et l'excuse sont tirés au début, et placés en position 2 et 3, et que le 1 et tiré après, et placé en 1
                    (les autres cartes de la main sont supposées identiques)
            On voit qu'on compte chaque main avec 3 bouts 3 fois au lieu d'une seule fois.
            Je vérifie cette analyse avec les nombres :
                probabilité incorrecte  = 0.1048951048951049
                probabilité   correcte  = 0.09293338240706661
                probabilité des 3 bouts = 0.005980861244019139
                écart entre les deux    = 0.011961722488038277 = 2 * probabilité d'avoir 3 bouts
                On voit que la différence est égale à 2 fois la probabilité d'avoir les 3 bouts
                On a donc bien compté chaque main à 3 bouts 3 fois au lieu d'une seule.
        CALCUL CORRECT :
            Il faut disjoindre les cas :
                CAS 1 = je tire 2 bouts, puis je tire les 13 autres cartes parmi les cartes restantes, mais SANS LE 3IÈME BOUT !
                CAS 2 = je tire 3 bouts, puis je tire les 12 autres cartes
                à noter qu'en fait, ça revient simplement à sommer les cas "exactement 2 bouts" et "exactement 3 bouts"...
                (autre possibilité : reprendre le calcul correct, et soustraire les cas où on a compté les 3 bouts trop de fois)
            Calculs :
                CAS 1
                    3 possibilités de choisir 2 bouts
                    15*14 possibilités de les positioner
                    A(13,75) pour les autres cartes SANS BOUT
                CAS 2
                    1 possibilités de choisir 3 bouts
                    15*14*13 possibilités de les positioner
                    A(12,75) pour les autres cartes SANS BOUT
                N = 3 * 15*14 * A(13,75)   +   15*14*13 * A(12,75)
                P = N / A(15,78)
            Valeur : 
                cf. le calcul probabiliste ci-après, plus simple
    Probabilités :
        P = p(B1 et B2 mais pas B3  OU  B1 et B3 mais pas B2  OU  B2 et B3 mais pas B1  OU  B1 et B2 et B3)  (mutuellement exclusifs )
        P = 15/78 * 14/77 * 63/76   +  15/78 * 63/77 * 14/76  +  63/78 * 15/77 * 14/76 + 15/78 * 14/77 * 13/76
        P = (3*14*15*63 + 15*14*13) / 78*77*76
        P = 14*15 * (3*63 + 13) / (78*77*76)
        P = (3*63 + 13) / 76    *    (14*15 / 78*77)
        P ~ 0.09293338240706661
    SIMULATEUR :
        P ~ 0.0929543333333
        (NOTE : le simulateur confirme donc que c'est mon calcul probabiliste qui est correct)


AU MOINS UN BOUT DANS MA MAIN :
    calcul direct :
        Ce coup-ci je ne m'embête pas, et je somme les probabilités :
        p(au moins un bout) = p(au moins 2 bouts) + p(exactement 1 bout)
        p(au moins un bout) = p(exactement 1 bout) + p(exactement 2 bouts) + p(exactement 3 bouts)
        P =   3 * 15*62*63 / 78*77*76   +   3 * 14*15*63 / 78*77*76   +    15*14*13 / 78*77*76
        P =  (3*15*62*63 + 3*14*15*63 + 15*14*13)  /  (78*77*76)
        P ~ 0.4780088332719912
    calcul indirect :
        P = p(au moins un bout dans ma main)
        P = 1 - p(aucun bout dans ma main)
        P = 1 - P2
        P2 est calculé plus haut à :
            P2 = 63*62*61  /  78*77*76
            P2 ~ 0.5219911667280088
        P = 1 - P2
        P ~ 0.4780088332719912
    COOL, je retrouve la même valeur avec les deux calculs.
    SIMULATEUR :
        P ~ 0.478449
        (NOTE : le simulateur confirme le calcul direct)
        
"""

exactement_0_bout = float(63*62*61)  /  (78*77*76)
exactement_1_bout = 3. * 15*62*63 / (78*77*76)
exactement_2_bout = 3. * 14*15*63 / (78*77*76)
exactement_3_bout = 1. * 15*14*13 / (78*77*76)

au_moins_1_bout = float(3*15*62*63 + 3*14*15*63 + 15*14*13)  /  (78*77*76)
au_moins_2_bout = 14*15 * float(3*63 + 13) / (78*77*76)

# Vérification :
assert_almost_equal(au_moins_1_bout, (exactement_3_bout + exactement_2_bout + exactement_1_bout))
assert_almost_equal(au_moins_2_bout, (exactement_3_bout + exactement_2_bout))
assert_almost_equal(exactement_0_bout, 1.0 - (exactement_3_bout + exactement_2_bout + exactement_1_bout))


print ""
print "Probabilités d'avoir dans sa main :"
print "   - exactement 0 bout = ", display_percent(exactement_0_bout)
print "   - exactement 1 bout = ", display_percent(exactement_1_bout)
print "   - exactement 2 bout = ", display_percent(exactement_2_bout)
print "   - exactement 3 bout = ", display_percent(exactement_3_bout)
print "   - au moins 1 bout   = ", display_percent(au_moins_1_bout)
print "   - au moins 2 bout   = ", display_percent(au_moins_2_bout)
