#!/usr/bin/env python
# coding: utf-8

from utils import C, A, f, assert_almost_equal, display_percent

"""
AU MOINS UNE COUPE FRANCHE À COEUR DANS MA MAIN :
    Dénombrement :
        A(15,78) mains au total
        A(15,64) mains sans coeur
        P(coupe à coeur) = A(15,64) / A(15,78)
        P(coupe à coeur) = (64!/49!)  /  (78!/63!)
        P(coupe à coeur) = 64*63*...*50  /  78*77*...*64
    Probabilités :
        P = p(coupe à coeur) = P(C1 pas un coeur  ET  C2 pas un coeur  ET ... ET  C15 pas un coeur)
        P = 64/78 * 63/77 * ... * 50/64
        P = 64*63*...*50  /  78*77*...*64
    OK, je retrouve la même valeur avec les 2 calculs...
    NOTE : ce calcul n'est pas très utile, car on peut très bien avoir une coupe à coeur et une autre à pique.


EXACTEMENT UNE COUPE FRANCHE (EX: À COEUR ET PAS D'AUTRE COUPE) DANS MA MAIN :
    Je veux une seule coupe franche -> je veux au moins une carte de chacune des 3 autres couleurs.
    Dénombrement :
        N(total) = A(15,78)
        N(total) = 78! / 63!
        N(total) = 78*77*...*64
        -----------------------
        14 possibilités de choisir un pique * 14 possibilités de choisir un carreau * 14 possibilités de choisir un trèfle.
        15*14*13 possibilités de positionner ce triplet.
        A(12,61) possibilités pour les autres cartes, choisies parmi celles restantes (78 - les 14 coeurs - les 3 autres cartes déjà choisies)
        N(exactement 1 coupe à coeur) = 14*14*14 * 15*14*13 * A(12,61)
        N(exactement 1 coupe à coeur) = 14*14*14 * 15*14*13 * 61!/49!
        N(exactement 1 coupe à coeur) = 14*14*14 * 15*14*13 * 61*60*...*50
        -----------------------
        P = N(exactement 1 coupe à coeur) / N(total) 
        P =      14*14*14 * 15*14*13 * 61*60*...*50   /   78*77*...*64
        P = 1. * 14*14*14 * 15*14*13 * 61*60*59*58*57*56*55*54*53*52*51*50   /   (78*77*76*75*74*73*72*71*70*69*68*67*66*65*64)
        -----------------------
        CE CALCUL EST FAUX :
            je compte certaines mains plusieurs fois :
                la main (As de pique, ..., 3 de pique) est comptée plusieurs fois
                une fois lorsqu'on a choisi l'As de pique comme premier pique qu'on a positionné au début, puis 13 autres cartes, et le 3 de pique comme dernière carte
                une fois lorsqu'on a choisi le 3 de pique comme dernière carte qu'on a positionné à la fin, puis 13 autres cartes, puis l'As de pique comme première carte
            du coup, mon calcul est faux...
    Probabiliste :
        P = p(exactement une coupe à coeur)
        P = p(aucun coeur et au moins 1 carte de chaque autrecouleur)
        Il va falloir introduire des sous-conditions :
            cond1 = C1 pas un coeur  ET  C2 pas coeur  ET  ...  ET  C15 pas un coeur
            cond2 = C1 un trèfle  OU  C2  un trèfle  OU  ...  OU  C15 un trèfle
            cond3 = C1 un carreau  OU  C2  un carreau  OU  ...  OU  C15 un carreau
            cond4 = C1 un pique  OU  C2  un pique  OU  ...  OU  C15 un pique
        P = p(cond1  ET  cond2  ET  cond3  ET  cond4)
        p(cond1) = 64/78 * 63/77 * ... * 50/64
        p(cond2) = on va utiliser les dénombrements pour calculer celle-ci :
            N(mains avec au moins 1 trèfle) =
                14 possibilités pour choisir le trèfle
                15 possibilités pour le positionner
                A(14,77) possibilités pour choisir les 14 autres cartes
            P(mains avec au moins 1 trèfle) = 14*15*A(14,77)
        Bon... Je m'arrête là pour le calcul probabiliste, car je pense que je n'aboutirais pas : 
            certes, j'ai réussi à exprimer mon problème sous la forme de 4 conditions "unitaires" (les cond1 à cond4 ci-dessus)
            MAIS déjà pour les calculer, je suis obligé de revenir aux dénombrements...
            ... et surtout, derrière, pour les assembler (car p(A et B) = p(A|B) * p(B)), je devrais AUSSI revenir aux dénombrements
            en effet, pour les probabilités conditionnelles, exprimer j'ai au moins 1 trèfle sachant que j'ai au moins 1 carreaux...
            ... se fera forcément avec du dénombrement...
    Simulateur :
        P(calculé) =  1. * 14*14*14 * 15*14*13 * 61*60*59*58*57*56*55*54*53*52*51*50   /   (78*77*76*75*74*73*72*71*70*69*68*67*66*65*64)
        P(calculé) ~ 
            



EXACTEMENT UNE COUPE FRANCHE QUELCONQUE DANS MA MAIN :
    Dénombrement, RAISONNEMENT FAUX :
        A(15,78) mains au total
        A(15,64) mains sans coeur + A(15,64) mains sans trèfle + A(15,64) mains sans pique + A(15,64) mains sans carreaux 
        P(au moins 1 coupe) = 4 * P(coupe à coeur)
        POURQUOI CE RAISONNEMENT EST FAUX : en faisant ça, je compte plusieurs fois les mains sans carreaux :
            p.ex. je compte une fois les mains sans coeur ET sans carreaux dans le calcul "mains sans coeur"
            (en effet, parmi les mains sans coeur, il y en a qui sont sans carreaux aussi)
            et je compte une deuxième fois les mains sans coeur et sans carreaux dans le calcul "mains sans carreaux)
            (en effet, parmi les mains sans carreaux, il y en a qui sont sans coeurs aussi)
    Dénombrement

"""

print "TODO : poursuivre, et trouver le raisonnement me permettant d'avoir une coupe quelconque."
