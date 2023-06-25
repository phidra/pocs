#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Cette POC démontre les syntaxes de création de context-managers.
#
# Le plus simple pour définir un context-manager est d'utiliser contextlib.
#
# cf. https://docs.python.org/2/reference/datamodel.html#context-managers
# cf. https://docs.python.org/2/library/stdtypes.html#typecontextmanager
# cf. https://docs.python.org/2/library/contextlib.html
#
########################################################################################################################

# Définition des contextmanagers :

class MyContext1(object):
    """
    Le paramètre permet de configurer le context-manager.
    Si on n'en a pas besoin, la méthode __init__ est optionnelle.
    """
    def __init__(self, myparameter):
        self.param = myparameter
    def __enter__(self):
        print "\tENTERING MyContext1, with parameter : ", self.param
    def __exit__(self, type, value, traceback):
        print "\tEXITING  MyContext1, with parameter : ", self.param

from contextlib import contextmanager
@contextmanager
def mycontext2(myparameter):
    print "\tENTERING mycontext2, with parameter : ", myparameter
    yield
    print "\tEXITING  mycontext2, with parameter : ", myparameter
    

# Exécution :

print "BEFORE"
with MyContext1("daparam1"):
    print "\tINSIDE   MyContext1"
print "AFTER"
print ""
print ""


print "BEFORE"
with mycontext2("daparam2"):
    print "\tINSIDE   mycontext2"
print "AFTER"
print ""
print ""



########################################################################################################################
# Fin du fichier.
########################################################################################################################
