#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Script utilitaire (servant à la poc) permettant d'afficher une valeur :
########################################################################################################################

import sys

# redis doit être installé :
try:
    import redis
except ImportError:
    print "ERROR - no redis package..."
    print ""
    print "pip install redis   # installe redis-py"
    sys.exit(10)

DATABASE = 4 # Arbirtrairement, on utilise la database 4

# argument obligatoire :
if len(sys.argv) <= 1:
    print "ERROR - mandatory argument (key)"
    sys.exit(20)

# affichage de la ou des clés demandées :
r = redis.StrictRedis(db=DATABASE)
for arg in sys.argv[1:]:
    print ""
    print "Clé demandée   = ", arg
    print "Valeur obtenue = ", r.get(arg)

########################################################################################################################
# Fin du fichier.
########################################################################################################################



