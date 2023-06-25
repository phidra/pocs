#!/usr/bin/env python
# -*- coding: utf-8 -*-

# On vérifie ce qui se passe lorsqu'un import écrase un nom défini.
# CONCLUSTION DE LA POC : comme attendu, l'import écrase la valeur de la variable définie auparavant.

MYSUPERVARIABLE = 42

from mymodule import MYSUPERVARIABLE

print "MYSUPERVARIABLE = ", MYSUPERVARIABLE
