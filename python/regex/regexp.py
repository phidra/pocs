#!/usr/bin/env python
# -*- coding: utf-8 -*-
################################################################################
#
# Tests d'expression régulière :
#
################################################################################

import re



################################################################################
### FONCTIONS 
################################################################################
def check_match(regexp, string):
    if re.match(regexp, string) is not None:
        print "\t[OUI] \"%s\" matche avec l'expression régulière \"%s\"" % (string, regexp)
    else:
        print "\t[NON] \"%s\" ne matche pas avec l'expression régulière \"%s\"" % (string, regexp)


################################################################################
### MAIN 
################################################################################

print "Test basique :"
check_match("^coucou$", "coucou")
check_match("^coucou$", " coucou")
check_match("^coucou$", " coucou ")
check_match("^[ \t]coucou[ \t]$", " coucou ")
print ""


print "Test avec une partie des nombres :"
check_match("[0-5][0-9] sec", "32 sec")
check_match("[0-5][0-9] [(sec)(min)]", "32 sec")
check_match("[0-5][0-9] [(sec)(min)]", "32 min")

################################################################################
# Fin du fichier.
################################################################################

