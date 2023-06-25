#!/usr/bin/env python
# -*- coding: utf-8 -*-

# On fait exister ces noms de variables pour ne pas introduire de différences parasites plus tard.
locals_before = locals_after = None

########################################################################################################################
# CAS 1 = un module ne déclarant pas __all__ dans le __init__ :
########################################################################################################################
def is_luke_available():
    """ Vérifie si le nom "luke" est disponible dans le namespace local. """
    try:
        luke.Luke().iamyourfather()
    except NameError:
        return False
    else:
        return True

# On montre que l'import * d'un module ne déclarant pas de __all__ dans le __init__ ne fait rien :
locals_before = dict(locals()).keys()
from module_without_all import *
locals_after = dict(locals()).keys()

assert(locals_before == locals_after)
assert(is_luke_available() == False)

# L'absence de __all__ n'empêche pas d'importer directement ce qu'on veut :
from module_without_all import luke
assert(is_luke_available() == True)


########################################################################################################################
# CAS 2 = un module déclarant __all__ dans le __init__ :
########################################################################################################################
def is_pouet_available():
    """ Vérifie si le nom "pouet" est disponible dans le namespace local. """
    try:
        pouet.Pouet().youpi()
    except NameError:
        return False
    else:
        return True

# On montre que l'import * d'un module déclarant __all__ dans le __init__ importe les noms du * :
locals_before = dict(locals()).keys()
from module_with_all import *
locals_after = dict(locals()).keys()

assert(locals_before != locals_after)
assert(is_pouet_available() == True)


print "POC OK, see details in script."
