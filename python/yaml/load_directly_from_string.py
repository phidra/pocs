#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Cette POC montre qu'il est possible de passer directement un flux à yaml.load (pas besoin de passer un fichier).
########################################################################################################################

import sys
import yaml # Nécessite d'installer le paquet python-yaml

yaml_stream = """
'Chef': 'M'
'Matricule courant': '007'
'Nom': 'BOND'
"""
mydict = yaml.load(yaml_stream)
for key, value in mydict.iteritems():
    print key, " → ", value

########################################################################################################################
# Fin du fichier.
########################################################################################################################
