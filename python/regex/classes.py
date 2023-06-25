#!/usr/bin/env python
# -*- coding: utf-8 -*-
################################################################################
#
# Tests d'expression régulière et d'utilisations des classes de caractères.
# L'enjeu est de vérifier que "\s" remplace bien n'importe quel caractère vide.
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

chaine1 = "mot1mot2" # sans espaces
chaine2 = "mot1       mot2" # avec espaces
chaine3 = "mot1\t\tmot2" # avec tabulations
chaine4 = "mot1\n\nmot2" # avec sauts de lignes
chaine5 = "mot1   \t\t \tmot2" # avec espaces et tabulations
chaine6 = "mot1   \n\n \nmot2" # avec espaces et sauts de ligne

print "NOTE : certaines chaînes comportent des tabulations ou des sauts de lignes..."
print ""

for tested_regex in ["mot1mot2", "mot1 *mot2", "mot1\t*mot2", "mot1\n*mot2", "mot1\s*mot2"]:
    print "Test avec la regex : ", tested_regex
    check_match(tested_regex, chaine1)
    check_match(tested_regex, chaine2)
    check_match(tested_regex, chaine3)
    check_match(tested_regex, chaine4)
    check_match(tested_regex, chaine5)
    check_match(tested_regex, chaine6)
    print ""

################################################################################
# Fin du fichier.
################################################################################

