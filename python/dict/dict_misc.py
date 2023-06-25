#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# POC diverses autour des dicts.
########################################################################################################################

mydict = {"John":"Lennon", "Ringo":"Starr", "Paul":"McCartney", "Georges":"Harrison"}

########################################################################################################################

def display(d):
    u""" Fonction utilitaire d'affichage du dictionnaire."""
    max_key = max(d.keys(), key=len)
    for key,value in d.iteritems():
        print "\t[{key}]  →  {value}".format(key=key.ljust(len(max_key)), value=value)

print "Le dictionnaire de la POC :"
display(mydict)
print ""

########################################################################################################################
# POC 1 = essayer de récupérer la valeur d'une clé inexistante lève une exception :
########################################################################################################################
print ""
try:
    unexisting_key = "poulet"
    print "On essaye de récupérer la clé \"{}\"".format(unexisting_key)
    got = mydict[unexisting_key]
except KeyError, e:
    print "[KeyError] : {e}".format(e=e)

########################################################################################################################
# POC 2 = récupérer une clé ou une valeur par défaut si elle n'existe pas :
########################################################################################################################
print ""
try:
    unexisting_key = "poulet"
    default_value = "citronnelle"
    print "On essaye de récupérer la clé \"{}\", avec la valeur par défaut \"{}\"".format(unexisting_key, default_value)
    got = mydict.get(unexisting_key, default_value)
    print "Ce qu'on a récupéré  :  \"{}\"".format(got)
except KeyError, e:
    print "[KeyError] : {e}".format(e=e)


########################################################################################################################
# Fin du fichier.
########################################################################################################################

