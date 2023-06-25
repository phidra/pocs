#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Cette POC montre que l'ordre de résolution d'accès aux attributs des objets est :
#       __getattribute__
#       l'accès direct à l'attribut s'il existe
#       __getattr__
#
# NOTE IMPORTANTE : __getattribute__ est IGNORÉ dans les old-style classes !
#
# NOTE : pour avoir une implémentation utile de __getattribute__, on peut :
#       soit lever AttributeError pour laisser la suite prendre le relais
#       soit appeler explicitement la méthode __getattribute__ de object.
#
# cf. https://docs.python.org/2/reference/datamodel.html#object.__getattribute__
#
########################################################################################################################


# __getattr__ intervient APRÈS la résolution des attributs existants :

class GetattrOnlyClass(object):
    def __init__(self):
        self.existingattribute = "existing attribute"
    def __getattr__(self, name):
        return "__getattr__ value"

getattronly = GetattrOnlyClass()
print "\nWith __getattr__ :"
print getattronly.existingattribute     # will print "existing attribute"
print getattronly.nonexistingattribute  # will print "__getattr__ value"


# __getattribute__ intervient AVANT la résolution des attributs existants :

class GetattributeClass(object):
    def __init__(self):
        self.existingattribute = "existing attribute"
    def __getattr__(self, name):
        return "__getattr__ value"
    def __getattribute__(self, name):
        return "__getattribute__ value"

getattribute = GetattributeClass()
print "\nWith __getattribute__ :"
print getattribute.existingattribute        # will print "__getattribute__ value"
print getattribute.nonexistingattribute     # will print "__getattribute__ value"


# __getattribute__ est IGNORÉ avec les old-style classes :

class GetattributeOldstyleClass():
    def __init__(self):
        self.existingattribute = "existing attribute"
    def __getattr__(self, name):
        return "__getattr__ value"
    def __getattribute__(self, name):
        return "__getattribute__ value"

getattributeoldstyle = GetattributeOldstyleClass()
print "\nWith __getattribute__ on old-style classes :"
print getattributeoldstyle.existingattribute        # will print "__getattribute__ value"
print getattributeoldstyle.nonexistingattribute     # will print "__getattribute__ value"


########################################################################################################################
# Fin du fichier.
########################################################################################################################
