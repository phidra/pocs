#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Ce que je veux vérifier avec cette POC :
#   - j'ai un script principal "main.py"
#   - j'ai un module externe "module.py"
#   - en externe, j'appelle le main en lui passant des arguments (ex: ./main.py Charlie Chaplin)
#   - le main appelle une fonction f du module
#   - dans le module, j'essaye de récupérer les arguments dans "sys.argv" (i.e. sans les passer à l'appel de f).
#   - est-ce que ça marche ?
#
########################################################################################################################

import module

module.f()


########################################################################################################################
# Fin du fichier.
########################################################################################################################
