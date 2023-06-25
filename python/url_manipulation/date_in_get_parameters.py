#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# QUESTION : passer une date dans un paramètre GET.
#
# Ce que je ne veux pas :
#
#       http://www.mysite.fr/?birthdate=06/09/1984
#
# Ce que je veux :
#
#       http://www.mysite.fr/?birthdate=06/09/1984
#
#
#
# RÉPONSE :
# Pour encoder :
#                   urllib.quote_plus(DATE)
# Pour décoder :
#                   urllib.unquote_plus(DATE)
#
#
# À noter que django a sa propre version de ces méthodes (cf. https://docs.djangoproject.com/en/dev/ref/utils/#module-django.utils.http)
#
########################################################################################################################

import urllib

raw = "21/12/2012"
encoded = urllib.quote_plus(raw)
decoded = urllib.unquote_plus(encoded)


print "raw              = \"{}\"".format(raw)
print "encoded          = \"{}\"".format(encoded)
print "decoded          = \"{}\"".format(decoded)
print "(raw == decoded) = {}".format(raw == decoded)


########################################################################################################################
# Fin du fichier.
########################################################################################################################

