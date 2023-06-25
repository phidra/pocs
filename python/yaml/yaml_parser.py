#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import yaml # Nécessite d'installer le paquet python-yaml

mydict = yaml.load(open(sys.argv[1]))
for key, value in mydict.iteritems():
    print key, " → ", value

########################################################################################################################
# Fin du fichier.
########################################################################################################################
