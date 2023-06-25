#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################

import sys

sys.dont_write_bytecode = True

from animals import Dog as AnimalDog
from hairybeings import Dog as HairyDog


mydog = AnimalDog()
mydog.walk()
mydog.jump()
mydog.bark()

myhairydog = HairyDog()
myhairydog.haircolour()

########################################################################################################################
# Fin du fichier.
########################################################################################################################
