#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Cette pypoc correspond à mes essais avec les décorateurs. cf. :
#     http://sametmax.com/comprendre-les-decorateurs-python-pas-a-pas-partie-1/
#     http://sametmax.com/comprendre-les-decorateur-python-pas-a-pas-partie-2/
#
# En résumé :
#     [POC 1] un décorateur est juste une fonction qui prend une fonction et retourne une autre fonction
#     [POC 2] pour décorer une fonction, on la passe par le décorateur (qui renvoie la fonction décorée)
#     [POC 3] souvent, on remplace la fonction initiale par la fonciton décorée   :   f = my_decorator(f)
#     [POC 4] sucre syntaxique : la syntaxe avec "@" permet de "remplacer" la fonction par sa version décorée, et ce DÈS SA DÉCLARATION
#
########################################################################################################################

# Voici une fonction simple :
def pouet():
    print "Poueeeeeeeeeeeeeeet !"

# Qui fonctionne :
print "========== SANS décoration =========="
pouet()
print ""

########################################################################################################################
# POC 1 = un décorateur est juste qqch qui prend une fonction, et retourne une autre fonction :
def my_decorator(f):
    def decorated_f():
        print "AVANT la fonction", f.__name__
        f()
        print "APRES la fonction", f.__name__
    return decorated_f

# POC 2 = pour décorer une fonction, on la passe par le décorateur (qui renvoie la fonction décorée) :
print "========== AVEC décoration =========="
decorated = my_decorator(pouet)
decorated()
print ""


########################################################################################################################

# Note : à ce stade, la fonction initiale reste accessible :
print "========== SANS décoration =========="
pouet()
print ""

# POC 3 = souvent, on ne veut utiliser QUE la fonction décorée, et on "remplace" donc la fonction initiale par la fonction décorée :
pouet = my_decorator(pouet)

# Dans ce cas, seule la fonction décorée existe (la fonction initiale n'est plus accessible) :
print "========== AVEC décoration =========="
pouet()
print ""

########################################################################################################################

# POC 4 = la syntaxe avec "@mydecorator" précédant une définition de fonction permet de décorer la fonction dès sa déclaration :
@my_decorator
def pouetbis():
    print "Et encore du pouet pouet !"


# La fonction est déjà décorée, sans avoir eu à la "remplacer" manuellement :
print "========== AVEC décoration =========="
pouetbis()
print ""


########################################################################################################################
# Fin du fichier.
########################################################################################################################
