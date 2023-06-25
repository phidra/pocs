#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Cette POC montre qu'une fois un module chargé en mémoire, c'est le même objet qui est renvoyé à chaque import.
# Et ce PEU IMPORTE où l'import a lieu (y compris depuis d'autres modules).
#
# Pour cela, ln importe datetime dans le main, en tête de a.py, et à l'intérieur d'une fonction de b.py.
# On vérifie pourtant qu'à chaque fois, l'objet module utilisé a le même id.
#
# Ceci explique notamment qu'on puisse monkey-patcher (ex: avec mock) un module depuis n'importe où dans l'application...
# ... et que le monkey-patching sera pourtant pris en compte partout.

import sys ; sys.dont_write_bytecode = True

import datetime
from a import display_a
from b import display_b

print "[MAIN] id(datetime) = {}".format(id(datetime))
display_a()
display_b()
