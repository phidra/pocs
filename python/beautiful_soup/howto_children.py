#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Cas pratiques d'utilisation de BeautifulSoup : on montre comment récupérer les enfants/parents.
########################################################################################################################

import bs4
soup = bs4.BeautifulSoup(open("sample3.html"))
 
# Affichage du document utilisé pour le test :
print "========================================================================================================================"
print "=== Document utilisé pour les tests :"
print "========================================================================================================================"
print (soup.prettify())
print "========================================================================================================================"
print ""

parent = soup.select(".parent")[0]

print "parent.contents : "
print "\t", parent.contents
for child in parent.contents:
    print "\t\t[TYPE={}]  {}".format(type(child).__name__, repr(child))
print ""

# ================================================================================

print "parent.children : "
print "\t", parent.children
for child in parent.children:
    print "\t\t[TYPE={}]  {}".format(type(child).__name__, repr(child))
print ""

# ================================================================================

print "parent.select('.child') :"
print "\t", parent.select('.child')
for child in parent.select('.child'):
    print "\t\t[TYPE={}]  {}".format(type(child).__name__, repr(child))
print ""

########################################################################################################################
# Fin du fichier.
########################################################################################################################



