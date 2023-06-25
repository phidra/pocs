#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# QUESTIONS :
#     [Q1]  est-il possible d'ajouter dynamiquement un attribut non-existant à un objet ?
#     [Q2]  et de supprimer dynamiquement des attributs définis par __init__ ?
#     [Q3]  quid des user-defined types ?
#
# RÉPONSES :
#     [Q1]  Oui !
#     [Q2]  Oui !
#     [Q3]  Non...
#
########################################################################################################################

class A:
    def __init__(self):
        self.via_init = "pouet"

    def display(self, msg):
        print "[DISPLAY] " + msg
        for i,j in self.__dict__.iteritems():
            print "\t", i, "\t", u" → ".encode("utf-8"), j
        print ""

a = A()
a.display(u"Après initialisation".encode("utf-8"))


################################################################################
# Réponses à la question Q1 :
a.externe = "Buzz"
a.display(u"Après ajout direct".encode("utf-8"))

setattr(a, "toystory", "Woody")
a.display(u"Après ajout via setattr".encode("utf-8"))


################################################################################
# Réponses à la question Q2 :
del(a.via_init)
a.display(u"Après ajout del d'un attribut défini dans __init__".encode("utf-8"))


################################################################################
# Réponses à la question Q3 :
b = "je suis un type de base"

try:
    b.toto = "marche pas"
except AttributeError, e:
    print "[EXCEPTION]", e

try:
    setattr(b, "essai", "ben non, marche toujours pas...")
except AttributeError, e:
    print "[EXCEPTION]", e


########################################################################################################################
# Fin du fichier.
########################################################################################################################
