#!/usr/bin/python
# -*- coding: utf-8 -*-
########################################################################################################################
# Démonstration de la syntaxe de base pour utiliser le logging.
# Les logs sont à la fois enregistrés dans "activity.log" ET affichés sur la console (dans des formats différents).
#
# Voir   :   http://docs.python.org/2/library/logging.html
# Voir   :   http://sametmax.com/ecrire-des-logs-en-python/ 
#
########################################################################################################################

import logging
from logging.handlers import RotatingFileHandler
 

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)

# Handler 1 = RotatingFile :
file_handler = RotatingFileHandler('activity.log', 'a', (1024*1024*1024), 10)
file_handler.setLevel(logging.DEBUG)
formatter = logging.Formatter('[%(asctime)s] %(message)s') # Voir   :   http://docs.python.org/2/library/logging.html#logrecord-attributes
file_handler.setFormatter(formatter)
logger.addHandler(file_handler)

# Handler 2 = StreamHandler
stream_handler = logging.StreamHandler()
stream_handler.setLevel(logging.DEBUG)
logger.addHandler(stream_handler)


# Code de test :
logger.info('Hello')
logger.warning('Testing %s', 'foo')


########################################################################################################################
# FIN DU FICHIER
########################################################################################################################
