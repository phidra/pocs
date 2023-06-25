#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Démonstration d'affichage propre d'un diff entre deux fichiers.
#
# Il faut passer par la comparaison de leurs lignes, et l'affichage ligne à ligne des différences.
#
########################################################################################################################

import path
import difflib

# Les textes à comparer :
SCRIPT_DIR = path.path(__file__).dirname()
f1 = SCRIPT_DIR / "sample1.txt"
f2 = SCRIPT_DIR / "sample2.txt"

# L'affichage de leurs différences :
lines1 = f1.text().splitlines()
lines2 = f2.text().splitlines()
for line in difflib.unified_diff(lines1, lines2, fromfile=f1.basename(), tofile=f2.basename(), lineterm=''):
    print line

########################################################################################################################
# Fin du fichier.
########################################################################################################################


