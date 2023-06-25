#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Cette POC montre l'utilité du mot-clé "global". La POC montre que :
#
#       - par défaut, les variables externes à un scope sont en lecture seule.
#       - l'utilisation de global permet de les rendre accessible à l'écriture
#
# NOTE : global rend accessibles les variables globales du MODULE (et non celles de ses sous-scopes)
#
# cf. https://www.python.org/dev/peps/pep-3104/
#
########################################################################################################################

pouet = 42
print "INITIALEMENT, pouet vaut : ", pouet

def WRONG_set_pouet(newvalue):
    """
    Mauvaise implémentation.
    En effet, ce n'est pas le pouet externe qui est modifié, mais une copie locale au scope.
    """
    pouet = newvalue
    print "[ WRONG ] APRÈS modification, en local, pouet vaut : ", pouet

def CORRECT_set_pouet(newvalue):
    """
    Bonne implémentation.
    C'est bien le pouet externe qui est modifié.
    """
    global pouet
    pouet = newvalue
    print "[CORRECT] APRÈS modification, en local, pouet vaut : ", pouet

# La fonction WRONG_set_pouet NE MODIFIE PAS la variable externe :
print ""
WRONG_set_pouet("Luke")
print "HORS DU SCOPE, pouet vaut : ", pouet

# La fonction CORRECT_set_pouet MODIFIE la variable externe :
print ""
CORRECT_set_pouet("Luke")
print "HORS DU SCOPE, pouet vaut : ", pouet


########################################################################################################################
# Fin du fichier.
########################################################################################################################
