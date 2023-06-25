#!/usr/bin/env python
# -*- coding: utf-8 -*-

# En gros, surcharger __cmp__ permet de définir toutes les comparaisons d'un coup.
# Si __cmp__ renvoie :
#   -1   l'objet de gauche est INFÉRIEUR à ce à quoi on le compare (objet de droite)
#    0   l'objet de gauche est ÉGAL      à ce à quoi on le compare (objet de droite)
#   +1   l'objet de gauche est SUPÉRIEUR à ce à quoi on le compare (objet de droite)

class Version(object):
    def __init__(self, major, minor):
        self.major = major
        self.minor = minor
    def __cmp__(self, value):
        if self.major > value.major:
            return 1
        elif self.major < value.major:
            return -1
        else:
            if self.minor > value.minor:
                return 1
            elif self.minor < value.minor:
                return -1
            else:
                return 0


v10 = Version(1, 0)
v11 = Version(1, 1)
other_v11 = Version(1, 1)
v20 = Version(2, 0)
v21 = Version(2, 1)

# v10 to others :
assert v10 < v11
assert v10 <= v11

assert v10 < v20
assert v10 <= v20

assert v10 < v21
assert v10 <= v21

# v11 to others :
assert v11 > v10
assert v11 >= v10

assert v11 < v20
assert v11 <= v20

assert v11 < v21
assert v11 <= v21

assert v11 == other_v11
assert v11 >= other_v11
assert v11 <= other_v11

# v20 to others :
assert v20 > v10
assert v20 >= v10

assert v20 > v11
assert v20 >= v11

assert v20 < v21
assert v20 <= v21

# v21 to others :
assert v21 > v10
assert v21 >= v10

assert v21 > v11
assert v21 >= v11

assert v21 > v20
assert v21 >= v20
