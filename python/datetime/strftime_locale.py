#!/usr/bin/env python
# -*- coding: utf-8 -*-
################################################################################
#
# Comment modifier la locale pour changer la sortie de strftime.
#
################################################################################

import datetime
import locale

now = datetime.datetime.now()

# On affiche et mémorise la locale :
old_locale = locale.getlocale(locale.LC_ALL)
print "OLD LOCALE = ", old_locale

# Avant changement de la locale, les dates sont a priori en anglais :
print "WITH OLD LOCALE = ", now.strftime("%d %B %Y")

# On change la locale (note : pour connaître les locales disponibles sous bash : "locale -a") :
locale.setlocale(locale.LC_ALL, "fr_FR.utf8")
print "FR  LOCALE = ", locale.getlocale(locale.LC_ALL)

# Après changement de la locale, les dates sont en français :
print "WITH FR  LOCALE = ", now.strftime("%d %B %Y")

# On reset la locale, et on montre que ça été pris en compte :
locale.setlocale(locale.LC_ALL, old_locale)
print "WITH OLD LOCALE = ", now.strftime("%d %B %Y")


################################################################################
# Fin du fichier.
################################################################################
