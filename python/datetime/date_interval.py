#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# POC démontrant comment définir des dates à intervalle régulier entre deux dates.
# On voit qu'on peut diviser l'année en 10 intervalles (chacun mesurant soit 36 jours, soit 37 jours).
#
########################################################################################################################

import datetime

################################################################################
# 
################################################################################

# On va parcourir une année (du 1er janvier 2001 au 1er janvier 2002) :
date_begin = datetime.date(2001,1,1)
date_end   = datetime.date(2002,1,1)

delta = date_end - date_begin
deci_delta = (delta / 10) # On parcourt l'année en pas de 10
dates = [date_begin + (x * deci_delta) for x in range(0, 10)]
print "On divise l'année en 10 intervalles."

previous = None
for date in dates:
    diff_with_previous = (date - previous if previous is not None else None)
    print "{}   [nombre de jours d'écart avec le précédent : {}]".format(date, diff_with_previous.days if previous is not None else "unknown")
    previous = date

print "Nombre de jours d'écart entre la dernière date ({}) et la borne supérieure ({}) : {}".format(dates[-1], date_end, (date_end - dates[-1]).days)


########################################################################################################################
# Fin du fichier.
########################################################################################################################
