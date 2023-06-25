#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Tests de redis avec des chaînes unicode.
#
# En résumé, redis-py encode automatiquement les chaînes unicode dans un encoding donné (par défaut, utf-8).
# redis stocke donc toujours des chaînes ENCODÉES
#
# Nécessite un serveur redis actif (apt-get install redis-server)
########################################################################################################################

import sys
import subprocess

# redis doit être installé :
try:
    import redis
except ImportError:
    print "ERROR - no redis package..."
    print ""
    print "pip install redis   # installe redis-py"
    sys.exit(1)
else:
    print "OK - package redis installed"
    print ""

DATABASE = 4 # Arbitrairement, cette POC est sur la database 4

r = redis.StrictRedis(db=DATABASE)
r.flushdb()

KEY = "mysuperkey"
KEY2 = "mysuperkey2"
USTRING = u"Leïa Skywalker"

########################################################################################################################
# POC 1 = redis peut stocker des bytestrings normalement (on encode avant de stocker dans redis, on décode en sortie)
########################################################################################################################

print "======== POC 1 ========"
print ""

print u"On stocke dans redis une chaîne UNICODE encodée en UTF-8 : [{}]".format(USTRING)

r.set(KEY, USTRING.encode("utf-8"))
print u"Ok pour le stockage dans redis."
print ""

print u"On récupère depuis redis la bytestring brute, et on l'affiche, puis on la décode et on affiche ce qui en résulte :"

rawgot = r.get(KEY)
decgot = unicode(r.get(KEY), "utf-8")

print u"RAWGOT = {} (type={})".format(repr(rawgot), type(rawgot).__name__)
# Affichera   RAWGOT = 'Le\xc3\xafa Skywalker' (type=str)

print u"DECGOT = {} (type={})".format(repr(decgot), type(decgot).__name__)
# Affichera   DECGOT = u'Le\xefa Skywalker' (type=unicode)

assert(decgot == USTRING)


print r"""


EXPLICATIONS :

Le caractère unicode dont le point de code est "EF" est : ï
>>> print u'\xef'.encode("utf-8")
ï

Et il s'encode en UTF-8 par "C3 AF" :
>>> print "\xc3\xaf".decode("utf-8")
ï

On voit donc que redis a stocké la bytestring correspondant à ce qui était passé ("Leïa Skywalker" encodé en UTF-8)
"""

########################################################################################################################
# POC 2 = on montre que le binding python de redis encode par défaut les chaînes de caractère en UTF-8 :
########################################################################################################################

print "======== POC 2 ========"
print ""

print u"On stocke dans redis une chaîne UNICODE non-encodée : [{}]".format(USTRING)
r.set(KEY, USTRING)
print u"Ok pour le stockage dans redis."
print ""

print u"On stocke dans redis une chaîne UNICODE encodée en UTF-8 : [{}]".format(repr(USTRING.encode("utf-8")))
r.set(KEY2, USTRING.encode("utf-8"))
print u"Ok pour le stockage dans redis."
print ""

print "On récupère les deux chaînes, et on montre qu'elles sont identiques :"
rawgot1 = r.get(KEY)
rawgot2 = r.get(KEY2)
print "rawgot1 = {}".format(repr(rawgot1))
print "rawgot2 = {}".format(repr(rawgot2))
print "rawgot1 == rawgot2  :  {}".format(rawgot1 == rawgot2)
assert(rawgot1 == rawgot2)

print ""
print u"On a prouvé qu'une chaîne UNICODE stockée dans redis était automatiquement encodée en UTF-8"

########################################################################################################################
# POC 3 = on montre qu'on peut paramétrer le binding python pour choisir l'encodage dans redis des chaînes unicode.
########################################################################################################################

print "======== POC 2 ========"
print ""

print u"On créée DEUX bindings python pour accéder au même serveur redis : l'un encodant en cp-1252, l'autre en utf-8."
r1252 = redis.StrictRedis(db=DATABASE, charset="cp1252")
rutf8 = redis.StrictRedis(db=DATABASE, charset="utf-8")
print ""

print u"On stocke dans redis1252 une chaîne UNICODE non-encodée : [{}]".format(USTRING)
r1252.set("1252", USTRING)
print u"Ok pour le stockage dans redis."
print ""

print u"On stocke dans redisutf8 une chaîne UNICODE non-encodée : [{}]".format(USTRING)
rutf8.set("utf8", USTRING)
print u"Ok pour le stockage dans redis."
print ""


print u"On va montrer que les chaînes BRUTES récupérées depuis redis sont différentes :"
rawgot1252 = r.get("1252")
rawgotutf8 = r.get("utf8")
print "rawgot1252 = {}".format(repr(rawgot1252))
print "rawgotutf8 = {}".format(repr(rawgotutf8))
print "rawgot1252 == rawgotutf8  :  {}".format(rawgot1252 == rawgotutf8)
assert(rawgot1252 != rawgotutf8)
print ""

print u"En revanche, on montre que les chaînes DÉCODÉES (avec le charset utilisé) sont identiques, et égales à l'unicode initiale"
decoded1252 = rawgot1252.decode("cp1252")
decodedutf8 = rawgotutf8.decode("utf-8")
print "decoded1252 = {}".format(repr(decoded1252))
print "decodedutf8 = {}".format(repr(decodedutf8))
print "decoded1252 == decodedutf8  :  {}".format(decoded1252 == decodedutf8)
assert(decoded1252 == decodedutf8)
print ""

########################################################################################################################
# Fin du fichier.
########################################################################################################################
