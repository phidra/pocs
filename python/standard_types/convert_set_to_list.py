#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Démonstration de la conversion d'un set en list (POC 1)
# À noter, la liste n'a peut-être rien à voir avec celle qui a donné naissance au set ! (POC 2)
#
########################################################################################################################


# POC 1 = on peut facilement convertir un set en list :
myset = set(["Luke", "Leia", "Anakin"])
mylist = list(myset)
print "mylist a pour type  [{type}] et contient :".format(type=type(mylist))
for i in mylist:
    print "- ", i
print ""
print ""


# POC 2 = la nouvelle liste peut être différente de celle qui a donné naissance au set :
seed_list = ["Frere jacques", "Frere jacques", "Dormez-vous ?", "Dormez-vous ?", "Sonnez les matines !"]
myset = set(seed_list)
mylist = list(myset)
print "SEED LIST = ", seed_list
print "MY LIST   = ", mylist


########################################################################################################################
# Fin du fichier.
########################################################################################################################
