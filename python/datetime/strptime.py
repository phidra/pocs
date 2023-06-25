#!/usr/bin/env python
# -*- coding: utf-8 -*-
################################################################################
#
# Vérification d'utilisation de strptime.
# Vérification aussi de ce qui se passe lorsque le format est invalide.
# (réponse : ça lève une exception)
#
################################################################################

import datetime

################################################################################
# Cas normal : on parse une date correcte.
################################################################################

input_date = "2014-01-08-09h03m21s"

parsed_date = datetime.datetime.strptime(input_date, "%Y-%m-%d-%Hh%Mm%Ss")

print "INPUT  DATE = ", input_date
print "PARSED DATE = ", parsed_date
print ""

if parsed_date == datetime.datetime(2014,1,8,9,3,21):
    print "OK, la date parsee est correcte :-)"
else:
    raise ValueError("Les dates sont differentes !")


################################################################################
# Cas aberrant : le format ne matche pas :
################################################################################

input_date = "2014-01-08-09h03mpouets"

try:
    parsed_date = datetime.datetime.strptime(input_date, "%Y-%m-%d-%Hh%Mm%Ss")
except ValueError, e:
    print ""
    print "OK, je vérifie que lorsque la date ne matche pas le format demandé on lève une exception :"
    print "\t", e
else:
    raise ValueError("Arf, on aurait du lever une exception sur le format...")

################################################################################
# Fin du fichier.
################################################################################

