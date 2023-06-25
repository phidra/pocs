#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# POC d'implémentation du singleton (et de démonstration d'utilisation des métaclasses par la même occasion).
# cf.  http://stackoverflow.com/questions/6760685/creating-a-singleton-in-python/6798042#6798042
#
########################################################################################################################

class Singleton(type):
    _instances = {}
    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            # Si l'instance n'existe pas encore, on la créé (en appelant classiquement type)
            cls._instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]

class MyConcreteSingleton(object):
    __metaclass__ = Singleton

print "Instantation of 3 objects..."
object1 = MyConcreteSingleton()
object2 = MyConcreteSingleton()
object3 = MyConcreteSingleton()

print "object1 id = ", id(object1)
print "object2 id = ", id(object2)
print "object3 id = ", id(object3)

########################################################################################################################
# Fin du fichier.
########################################################################################################################
