#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# MA QUESTION :
#       - on "déclare" une nouvelle variable à l'intérieur d'un "bloc" (if, try, while, with, function etc.)
#       - sachant que la variable n'existait pas avant le bloc, est-elle utilisable après la sortie du bloc ?
#
# RÉPONSE :
#       - une déclaration (en fait assignation d'une variable à un objet) se fait toujours pour le scope courant
#       - en dehors du "bloc" d'une fonction, les autres blocs n'ont pas de scope propre
#       - par conséquent, pour tous les cas ci-dessous (sauf la fonction), toute variable définie dans le bloc existe à l'extérieur
#       - pour assigner une variable externe au scope de la fonction, il est nécessaire d'identifier la variable avec "global"
#
#
# http://docs.python.org/2/tutorial/classes.html#python-scopes-and-name-spaces
# http://docs.python.org/2/reference/executionmodel.html#naming-and-binding
#
########################################################################################################################


################################################################################
# BLOC FOR :
for i in range(5):
    var_for = "pouet"
print "var_for = ", var_for


################################################################################
# BLOC TRY/CATCH :
try:
    var_try = "youpi"
except:
    pass
print "var_try = ", var_try

################################################################################
# BLOC IF :
import random
if random.randint(0,1):
    var_if = "Buzz"
else:
    var_if = "Lightning"
print "var_if  = ", var_if

################################################################################
# BLOC WITH :

with open("/etc/fstab", "r") as f:
    var_with = "ChupaChups"
print "var_with= ", var_with

################################################################################
# DANS UNE FONCTION :
def f1():
    var_f1 = "Vers l'infini et au dela"

f1()
try:
    print "var_f1  = ", var_f1
except NameError, e:
    print "CA NE MARCHE PAS ({exception})".format(exception = e)

################################################################################
# DANS UNE FONCTION AVEC global :
def f2():
    global var_f2
    var_f2 = "C'est Raoul, le pitbull"
f2()
print "var_f2  = ", var_f2

########################################################################################################################
# Fin du fichier.
########################################################################################################################
