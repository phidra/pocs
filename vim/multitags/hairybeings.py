#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################

import sys

sys.dont_write_bytecode = True


class Hairy:
    def myhairs(self):
        return "I've got a lot of hair !"

    def haircolour(self):
        raise NotImplementedError()


class Dog(Hairy):
    def haircolour(self):
        return "Brown"


########################################################################################################################
# Fin du fichier.
########################################################################################################################
