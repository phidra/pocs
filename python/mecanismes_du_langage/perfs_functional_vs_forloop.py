#!/usr/bin/env python
# coding: utf-8

"""
L'objectif de cette POC est de comparer les perfs d'exécution pour une opération simple.
INPUT = une liste de dicts, dont la clé "mode" peut être "walk", "car" (ou "transport")
MODIF = si le champ "mode" est à "car", on le passe à "walk".

On compare 3 façons de faire :
  - avec une boucle for
  - avec une comprehension list
  - avec maps

On montre au passage l'utilisation de timeit :
  - appel d'une chaîne à tester :
      t = timeit.timeit("[('pair' if i % 2 == 0 else 'impair') for i in range(100)]")
  - l'utilisation de setup :
      t = timeit.Timer('f(mynumber)', setup='mynumber = 42').timeit()
  - syntaxe pour tester une fonction :
      t = timeit.Timer('myfunc(myargs)', setup='from __main__ import myfunc').timeit()

RÉSULTATS DE LA POC : map est plus rapide. Résultat typique :
    Replacement using map       =  1.31645011902
    Replacement using forloop   =  1.9123468399
    Replacement using comp list =  1.54056406021
"""

import random
import timeit


setup = '''
import random
MODES = ("car", "walk", "waypoint", "transport")
TESTDATA = [{"id": x, "mode": random.choice(MODES)} for x in range(10000)]
replace = lambda d: d.update({"mode": "walk"}) if d["mode"] == "car" else d
'''

def with_forloop(testdata, replace):
    toreturn = []
    for item in testdata:
        toreturn.append(replace(item))
    return testdata

def with_map(testdata, replace):
    return map(replace, testdata)

def with_comp(testdata, replace):
    return [replace(d) for d in testdata]




t_map = timeit.Timer('with_map(TESTDATA, replace)', setup=setup + 'from __main__ import with_map').timeit(number=1000)
print "Replacement using map       = ", t_map

t_forloop = timeit.Timer('with_forloop(TESTDATA, replace)', setup=setup + 'from __main__ import with_forloop').timeit(number=1000)
print "Replacement using forloop   = ", t_forloop

t_comp = timeit.Timer('with_comp(TESTDATA, replace)', setup=setup + 'from __main__ import with_comp').timeit(number=1000)
print "Replacement using comp list = ", t_comp

