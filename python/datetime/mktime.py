#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Preuve d'utilisation de la timezone du système par mktime.
########################################################################################################################
#
# mktime permet de convertir un struct_time en timestamp
# 
# or, un struct_time est "naive" (il ne connaît presqu'aucune information permettant d'obtenir un horodatage absolu, on a uniquement "isdst")
# mktime, qui calcule un timestamp (donc un horodatage absolu) doit donc prendre une référence pour localiser le struct_time dans le temps
# ce qui me paraîssait le plus logique, c'est qu'il considère que le struct_time est exprimé en UTC
# la POC montre que ce n'est pas le cas, et que mktime considère que son entrée est EN HEURE LOCALE
# (comme l'indique la doc : https://docs.python.org/2/library/time.html#time.mktime)
# on montre en effet que mktime utilise la timezone du système pour faire son calcul : à input équivalent, le résultat varie avec la timezone utilisée
#
########################################################################################################################

import os
import time
import datetime

########################################################################################################################
# Preuve que le résultat de mktime varie si la timezone du système varie :
########################################################################################################################

ten_hours_after_epoch = (1970,1,1,10,0,0,0,0,0)

# Dans la timezone actuelle du système :
print u"Dans la timezone actuelle du système  :  ", time.mktime(ten_hours_after_epoch)

# Dans la timezone UTC :
os.environ["TZ"] = "UTC"
time.tzset()
print u"Dans la timezone UTC                  :  ", time.mktime(ten_hours_after_epoch)

# Dans la timezone UTC :
os.environ["TZ"] = "Europe/Paris"
time.tzset()
print u"Dans la timezone \"Europe/Paris\"       :  ", time.mktime(ten_hours_after_epoch)

print u"""
On voit que mktime utilise la timezone du système pour faire son calcul, puisque celui-ci varie avec celle-là...

À noter qu'on peut aussi faire cette POC sous bash :
    export TZ="UTC"
    python -c "import time ; print time.mktime((1970,1,1,10,0,0,0,0,0));"
    export TZ="Europe/Paris"
    python -c "import time ; print time.mktime((1970,1,1,10,0,0,0,0,0));"
"""

########################################################################################################################
# Une autre façon de voir les choses, c'est de considérer que le résultat de mktime est INCORRECT si on ne lui passe pas une heure locale. 
########################################################################################################################

timestamp_actuel = int(time.time())
timestamp_calcule_avec_mktime_en_lui_passant_lheure_actuelle_LOCALE = int(time.mktime(datetime.datetime.now().timetuple()))
timestamp_calcule_avec_mktime_en_lui_passant_lheure_actuelle_UTC    = int(time.mktime(datetime.datetime.utcnow().timetuple()))

print u"========================================================================================================================"
print u""
print u"Preuve que le timestamp renvoyé par mktime est INCORRECT si on ne lui passe pas l'heure locale :"
print u"\tTimestamp actuel (réel)                                      :", timestamp_actuel
print u"\tTimestamp calculé par mktime (en lui passant l'heure LOCALE) :", timestamp_calcule_avec_mktime_en_lui_passant_lheure_actuelle_LOCALE
print u"\tTimestamp calculé par mktime (en lui passant l'heure UTC)    :", timestamp_calcule_avec_mktime_en_lui_passant_lheure_actuelle_UTC

########################################################################################################################
# Fin du fichier.
########################################################################################################################
