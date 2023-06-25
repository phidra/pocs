#!/usr/bin/env python
# coding: utf-8

# Cette POC montre la définition et l'utilisation d'un générateur.
# Le principe est d'itérer sur les lignes d'un fichier.
# De façon classique, une fonction renvoie les lignes d'un fichier.
# Avec le générateur, on yield les lignes les unes après les autres.

# Le principe global est que l'exécution du générateur linéaire, elle est simplement mise en pause au moment du yield.

# À noter qu'AUCUN code n'est exécuté à l'appel de la fonction !
# (ce n'est qu'au moment de l'itération qu'on exécute le code, jusqu'au premier yield)

# On voit notamment que tout le code du générateur est exécuté (y compris le code qui suit le dernier yield)

PATH = "/tmp/pypoc_datafile"

# Préalable, on écrit un fichier sur lequel la POC va itérer :
with open(PATH, "w") as f:
    f.write("Luke\nLeia\nAnakin\nObi-wan")


# Fonction classique :
def classic_presenter(path):
    with open(path, "r") as f:
        return [repr(line) for line in f]


# Fonction avec générateur :
def generator_presenter(path):

    # Ce code N'EST PAS exécuté à l'appel de la fonction (il ne l'est qu'au moment de l'itération réelle) :
    print "=== (generator) BEGIN"

    with open(path, "r") as f:
        for line in f:
            yield "(generator) The value of the line is : " + repr(line)

    # Tout le code du générateur est exécuté, y compris le code après le dernier yield :
    print "=== (generator) END"


# MAIN :
print "classic_presenter     = ", classic_presenter
print "generator_presenter   = ", generator_presenter
print "generator_presenter() = ", generator_presenter(PATH)
print ""

# Génération "classique" :
print "Itération \"classique\" :"
for line in classic_presenter(PATH):
    print line

print ""

print "Itération avec générateur :"
for line in generator_presenter(PATH):
    print line
