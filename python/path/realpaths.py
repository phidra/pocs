#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# POC pour savoir ce que font :
#   - realpath
#   - abspath
#   - expand
#   - readlink
#   - readlinkabs
#
# Notamment : quelles différences entre :
#   - p1 = mypath.expand().abspath()
#   - p2 = mypath.expand().realpath()
#
# Réponse :
#   - aucune différence si mypath n'est pas (ou ne contient pas) de lien symbolique.
#   - si mypath est un lien symbolique :
#        p1 contiendra le chemin absolu vers le lien symbolique
#        p2 contiendra le chemin absolu vers le fichier pointé par le lien symbolique
#
########################################################################################################################

import path
import os
from tempfile import mkstemp

########################################################################################################################


test1 = path.path("/chemin/absolu/vers/un/fichier")
test2 = path.path("chemin_relatif_vers_un_fichier")
test3 = path.path("~/avec/un/tilde")


print("NOTE : os.getcwd() = " + os.getcwd())
for test in (test1, test2, test3):
    print("")
    print(test)
    print("\tabspath     = " + test.abspath())
    print("\trealpath    = " + test.realpath())
    print("\texpand      = " + test.expand())

# Tests avec les liens symboliques :
_, realfile = mkstemp(suffix="_REAL")
_, link = mkstemp(suffix="_LINK")
link = path.path(link)
realfile = path.path(realfile)
link.remove_p()
realfile.symlink(link)


print("")
print(u"À ce stade, on a un fichier réel ({real}), et un lien symbolique qui pointe dessus ({link}).".format(real=realfile, link=link).encode("utf-8"))
print("")
test = link
print(test)
print("\tabspath     = " + test.abspath())
print("\trealpath    = " + test.realpath())
print("\texpand      = " + test.expand())
print("\treadlink    = " + test.readlink())
print("\treadlinkabs = " + test.readlinkabs())

########################################################################################################################
# Fin du fichier.
########################################################################################################################


