#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
#
# Cette POC montre la différence entre mock.patch et mock.patch.object, sur deux exemples :
#    - un objet d'un module importé (mymodule)
#    - un objet local (myclass)
#
#      cf. http://stackoverflow.com/questions/29152170/what-is-the-difference-between-mock-patch-object-and-mock-patch
#
# CONCLUSIONS DE LA POC :
#  
#     mock.patch.object patche un NOM D'ATTRIBUT D'UN OBJET EXISTANT
#     (une référence déjà connue dans le scope ; p.ex. un module déjà importé)
#  
#         >>> with mock.patch.object(mymodule, "mydate", return_value=ARBITRARY_DATE):
#         >>>     pass
#         >>> with mock.patch.object(mylocalclass, "compute", return_value=ARBITRARY_COMPUTED):
#         >>>     pass
#  
#  
#     mock.patch patche un chemin de résolution de nom (donc une string) :
#  
#         >>> with mock.patch("mymodule.mydate", return_value=ARBITRARY_DATE):
#         >>>     pass
#         >>> with mock.patch("__main__.mylocalclass.compute", return_value=ARBITRARY_COMPUTED):
#         >>>     pass
#
########################################################################################################################

import sys ; sys.dont_write_bytecode = True
from datetime import date
import mock
import mymodule

########################################################################################################################
# Patching a module's function :
########################################################################################################################

ARBITRARY_DATE = date(1984, 9, 6)

print "[    no mock      ] mydate = ", mymodule.mydate()
with mock.patch.object(mymodule, "mydate", return_value=ARBITRARY_DATE):
    print "[mock.patch.object] mydate = ", mymodule.mydate()
with mock.patch("mymodule.mydate", return_value=ARBITRARY_DATE):
    print "[   mock.patch    ] mydate = ", mymodule.mydate()

########################################################################################################################
# Patching a local object :
########################################################################################################################

class mylocalclass():
    @classmethod
    def compute(self):
        return "pouet"

ARBITRARY_COMPUTED = "ArBiTrArY"

print "[    no mock      ] computed = ", mylocalclass.compute()
with mock.patch.object(mylocalclass, "compute", return_value=ARBITRARY_COMPUTED):
    print "[mock.patch.object] computed = ", mylocalclass.compute()
with mock.patch("__main__.mylocalclass.compute", return_value=ARBITRARY_COMPUTED):
    print "[   mock.patch    ] computed = ", mylocalclass.compute()
