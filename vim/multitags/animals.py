#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################

import sys
sys.dont_write_bytecode = True

class Animal:
    def walk(self):
        raise NotImplementedError()
    def jump(self):
        raise NotImplementedError()

class Dog(Animal):
    def walk(self):
        print "Dog is walking..."
    def jump(self):
        print "Dog is jumping..."
    def bark(self):
        print "Dog is barking..."

########################################################################################################################
# Fin du fichier.
########################################################################################################################
