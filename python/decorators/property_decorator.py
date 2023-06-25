#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Cette pypoc correspond à mes essais autour du décorateur "property".
# Utilisée comme décorateur, la built-in property permet de "transformer" une fonction en attribut.
# cf. http://docs.python.org/2/library/functions.html#property
# 
#     [POC 1] = property est une builtin
#     [POC 2] = property ne marche qu'avec des new-style classes (dérivant de "object") → [EDIT : en fait, en lecture seule , ça a l'air de marcher...]
#     [POC 3] = avec property, on peut simuler un attribut avec une fonction de classe
#     [POC 4] = si on essaye de modifier une simulation d'attribut qui n'a pas défini de setter, on se prend une exception
#     [POC 5] = il est possible de définir aussi les setters et getters
#
########################################################################################################################


# POC 1 = on prouve que "property" est une builtin :
########################################################################################################################

import __builtin__
assert(property is  __builtin__.property)


# POC 2 = elles ne fonctionnent qu'avec les new-style classes [EDIT : ça a l'air de fonctionner tout de même... ?]:
########################################################################################################################

class AccessCounterOldStyle:
    u""" Noter l'absence d'héritage de object."""
    def __init__(self):
        self._counter = 0

    @property
    def current_count(self):
        self._counter += 1
        return self._counter

a = AccessCounterOldStyle()
print "Current count for object \"a\" = ", a.current_count
print "Current count for object \"a\" = ", a.current_count
print "Current count for object \"a\" = ", a.current_count
b = AccessCounterOldStyle()
print "Current count for object \"b\" = ", b.current_count
print "Current count for object \"b\" = ", b.current_count
print "Current count for object \"a\" = ", a.current_count
print ""


# POC 3 = on a accès à une fonction d'une classe comme un attribut :
########################################################################################################################

class AccessCounterNewStyle(object):
    def __init__(self):
        self._counter = 0

    @property
    def current_count(self):
        self._counter += 1
        return self._counter

c = AccessCounterOldStyle()
print "Current count for object \"c\" = ", c.current_count
print "Current count for object \"c\" = ", c.current_count
print "Current count for object \"c\" = ", c.current_count
d = AccessCounterOldStyle()
print "Current count for object \"d\" = ", d.current_count
print "Current count for object \"d\" = ", d.current_count
print "Current count for object \"c\" = ", c.current_count
print ""


# POC 4 = si on essaye de modifier une property qui n'a pas défini de setter, on se prend une exception :
########################################################################################################################

e = AccessCounterNewStyle()
print "Current count for object \"e\" = ", e.current_count
print "Current count for object \"e\" = ", e.current_count
print "Current count for object \"e\" = ", e.current_count
try:
    e.current_count = 42
    print "After set, Current count for object \"e\" = ", e.current_count
except AttributeError, e:
    print "[EXCEPTION] (quand on essaye de setter une property) : ", e
print ""



# POC 5 = on peut aussi définir les setters et deletters :
########################################################################################################################

""" Rappel :

            @property
            def f():
                # ...something...

    est équivalent à :

            def f():
                # ...something...
            f = property(f)
"""

# Je vois trois façons de définir les setters et getters (et la deuxième ne marche pas) :

class Essai(object):
    def __init__(self):
        self._komoku = 0
        self._hoshi = 0
        self._sansan = 0

    # Façon 1 = en zappant le sucre syntaxique (permettant d'utiliser un décorateur avec "@") :
    def komoku_getter(self):
        print "[KOMOKU GETTER]"
        return self._komoku
    def komoku_setter(self, value):
        print "[KOMOKU SETTER]"
        self._komoku = value
    def komoku_deleter(self):
        print "[KOMOKU DELETER]"
        del self._komoku
    komoku = property(komoku_getter, komoku_setter, komoku_deleter)

    ## Façon 2 = en utilisant le sucre syntaxique, en passant le getter et setter comme argument du décorateur :
    ## EDIT : ça ne marche pas...
    #def hoshi_setter(self):
    #    self._hoshi = value
    #def hoshi_deleter(self, value):
    #    del self._hoshi
    #@property(hoshi_setter, hoshi_deleter)
    #def hoshi_getter(self):
    #    return self._hoshi

    # Façon 3 = en utilisant la façon spécifique à la builtin property, conçue pour ça :
    # NOTE : apparemment, les trois fonctions (getter, setter, deleter) doivent avoir le même nom...
    @property
    def sansan(self):
        print "[SANSAN GETTER]"
        return self._sansan
    @sansan.setter
    def sansan(self, value):
        print "[SANSAN SETTER]"
        return self._sansan
    @sansan.deleter
    def sansan(self):
        print "[SANSAN DELETER]"
        return self._sansan

obj = Essai()

obj.komoku = 42
a = obj.komoku
del obj.komoku

print ""

obj.sansan = 3.14
a = obj.sansan
del obj.sansan





########################################################################################################################
# Fin du fichier.
########################################################################################################################
