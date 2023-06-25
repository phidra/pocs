#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Explication sur l'accès par une fonction aux variables extérieures à son scope :
#     [POC 1] une fonction a toujours accès aux variables extérieures à son scope
#     [POC 2] elle y a accès en lecture ou écriture
#     [POC 3] c'est la valeur de la variable extérieur définie au moment de l'APPEL de la fonction (et non de sa déclaration) qui compte
# 
# L'article sur les closures de Sam & Max (http://sametmax.com/closure-en-python-et-javascript/) se plante sur deux points :
#     1. ce qu'il fait dans l'article n'est pas une closure, mais simplement l'utilisation de variables globales
#     2. celles-ci sont accessibles en lecture ET EN ÉCRITURE (il est en revanche impossible de les réaffecter ou de les détruire → [POC 4])
# 
# CLOSURE :
#     Lorsqu'on utilise une variable globale, il y a donc une dépendance entre la fonction et la valeur de la variable au moment de l'appel [POC 3]
#     Cette dépendance globale n'est pas bonne du point de vue de la conception, et est à éviter.
#     Pour faire que f utilise la valeur de la variable globale au moment de la DÉCLARATION et non de l'appel, il faut une closure → [POC 5, POC 6]
#     Il est possible d'inspecter le contenu des closures d'une fonction → [POC 7]
#
########################################################################################################################


################################################################################
# POC 1 = une fonction a accès aux variables extérieures à son scope
print ("\n\n=== POC 1 ===")

external1 = 42

def f1():
    print external1

f1()  # Affiche 42


################################################################################
# POC 2 = elle y a accès en lecture ET EN ÉCRITURE
print ("\n\n=== POC 2 ===")

# NOTE : pour cette POC, attention à ne pas confondre :
#       - [autorisé] la modification de l'objet (ici : external2.append)
#       - [interdit] la réaffectation de la variable (external2 = "autre chose"), qui conduit à la création d'une nouvelle variable (locale)

external2 = ["a", "b", "c"]

def f2():
    external2.append("d")

print "+".join(external2)  # Affiche "a+b+c"
f2()
print "+".join(external2)  # Affiche "a+b+c+d"


################################################################################
# POC 3 = c'est la valeur de la variable extérieure au moment de l'APPEL de la fonction (et non de sa déclaration) qui compte
print ("\n\n=== POC 3 ===")

def f3():
    print external3

external3 = 42
f3()  # affiche 42

external3 = 3.14
f3()  # affiche 3.14

del external3

try:
    f3()  # Interdit, external3 n'existe plus !
except NameError, e:
    print "[EXCEPTION]", e


################################################################################
# POC 4 : même si les variables extérieures sont accessibles en lecture et écriture, il est impossible de les réaffecter ou de les détruire
print ("\n\n=== POC 4 ===")

def f4():
    print external4

def g4():
    del external4

external4 = 42
f4()  # Autorisé, affiche 42

try:
    g4()  # Interdit !
except UnboundLocalError, e:
    print "[EXCEPTION]", e


################################################################################
# POC 5 : pour utiliser la valeur de la variable au moment de la DÉCLARATION, il faut une closure :
print ("\n\n=== POC 5 ===")

def generateur_de_f(external5):
    def f():
        print external5
    return f

ma_variable_globale = 42
f5 = generateur_de_f(ma_variable_globale)

f5()  # Affiche 42

ma_variable_globale = 3.14
f5()  # Affiche toujours 42 !

del ma_variable_globale
f5()  # Affiche toujours 42, même si ma_variable_globale n'existe plus... En effet, ce n'est plus elle qui est utilisée  !


################################################################################
# POC 6 : autre syntaxe pour créer la closure directement à partir de la variable globale (plutôt qu'en paramètre du builder)
print ("\n\n=== POC 6 ===")

def generateur_de_f():
    closed = external6
    def f():
        print closed
    return f

external6 = 42
f5 = generateur_de_f()

f5()  # Affiche 42

external6 = 3.14
f5()  # Affiche toujours 42 !

del external6
f5()  # Affiche toujours 42, même si external6 n'existe plus... En effet, ce n'est plus elle qui est utilisée  !


################################################################################
# POC 7 : Il est possible d'inspecter le contenu des closures.
print ("\n\n=== POC 7 ===")

external7 = "luke"

def a():
    print external7

print "a.__closure__ = ", a.__closure__


def builder():
    closed = external7
    def wrapped():
        print closed
    return wrapped

b = builder()
print "b.__closure__ = ", b.__closure__
print "b.__closure__[0].cell_contents = ", b.__closure__[0].cell_contents

########################################################################################################################
# Fin du fichier.
########################################################################################################################
