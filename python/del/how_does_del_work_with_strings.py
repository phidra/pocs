#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# QUESTION = comment fonctionne del ? Supprime-t-il l'élément détruit ?
# RÉPONSE  = NON ! del se contente de supprimer l'association d'un objet à la variable le référençant.
#            L'objet est détruit (automatiquement par le garbace collector) lorsque la dernière référence est détruite.
#            Par contre, je n'arrive pas à supprimer définitivement une chaîne...
#
########################################################################################################################

import os
import subprocess
import copy

# Fonction utilitaire, renvoyant la mémoire consommée :
def memory_consumption_in_kib():
    command = "ps u -p " + str(os.getpid()) + " | awk '{sum=sum+$6}; END {print sum}'"
    p = subprocess.Popen(command, shell = True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate()
    if p.returncode != 0:
        raise ValueError("non-zero returncode ({rc}), with stdout=[{out}] and stderr=[{err}]".format(rc=p.returncode, out=out, err=err))
    return int(out.rstrip("\n"))


print "mémoire initiale                =", memory_consumption_in_kib(), "kib"
a = "Je suis une super patate de la mort qui tue tout, avec un texte suffisamment long pour me valoir le fait d'augmenter au moins d'un kio la mémoire consommée, ah ben oui, tiens, c'est vrai que c'est compliqué de taper de longs textes, mais je ne doute pas qu'avec un peu d'imagination, je puisse obtenir un texte suffisamment long, quitte à taper des trucs que je connais par cœur, comme des paroles de chansons, par exemple, tenez : You could be my uninteneded, choice to live my life extended, etc."


print "après création d'une chaîne     =", memory_consumption_in_kib(), "kib"
b = a
print "après copie simple de la chaîne =", memory_consumption_in_kib(), "kib"
c = copy.copy(a)
print "après copy.copy de la chaîne    =", memory_consumption_in_kib(), "kib"
d = copy.deepcopy(a)
print "après deepcopy de la chaîne     =", memory_consumption_in_kib(), "kib"
print ""

print "après destruction de d          =", memory_consumption_in_kib(), "kib"
del(d)
print "après destruction de c          =", memory_consumption_in_kib(), "kib"
del(c)
print "après destruction de b          =", memory_consumption_in_kib(), "kib"
del(b)
print "après destruction de a          =", memory_consumption_in_kib(), "kib"
del(a)


########################################################################################################################
# Fin du fichier.
########################################################################################################################

