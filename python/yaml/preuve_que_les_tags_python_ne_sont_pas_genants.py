#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Pour prouver que les tags ne sont pas gênants, on va charger deux dicts à partir de deux fichiers yaml différents :
#   - le premier, directement issu d'un dump d'un dictionnaire python comportant des unicode, donc AVEC des tags python
#   - le deuxième, correspondant au premier SANS les tags python
# 
# On montre que les deux load sont identiques (quelque part, ça montre que les tags ne servent à rien).
########################################################################################################################

import sys
import yaml # Nécessite d'installer le paquet python-yaml

dict_from_yamlfile_with_tags    = yaml.load(open("montest_AVEC_TAGS.yaml"))
dict_from_yamlfile_without_tags = yaml.load(open("montest_SANS_TAGS.yaml"))

print ""
print "Dict loadé à partir du fichier AVEC tags :"
for key, value in dict_from_yamlfile_with_tags.iteritems():
    print key, " → ", value
print ""


print ""
print "Dict loadé à partir du fichier SANS tags :"
for key, value in dict_from_yamlfile_without_tags.iteritems():
    print key, " → ", value
print ""

print ""
print "Les deux dicts sont-ils identiques ? ", dict_from_yamlfile_without_tags == dict_from_yamlfile_with_tags


########################################################################################################################
# Fin du fichier.
########################################################################################################################
