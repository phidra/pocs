#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Tests de création d'une archive zip.
#
# cf.  https://docs.python.org/2/library/shutil.html#archiving-operations
#
# Ce que la POC m'apprend :
#    Il suffit d'archiver un fichier en incluant son répertoire dans le chemin précisé pour "agréger" les fichiers de l'archive au sein d'un répertoire.
#    Dit autrement, en faisant comme ça, lorsqu'on extrait l'archive, tous les fichiers extraits se retrouvent rassemblés dans un répertoire.
#    Il est possible de ne pas comprimer les fichiers, uniquement de les stocker (via zip.ZIP_STORE)
#    La POC prouve que les deux façons de générer une archive (shutil.make_archive et zipfile) génèrent des archives identiques.
#
########################################################################################################################

import shutil
import zipfile
import path

########################################################################################################################
# CONFIG :
########################################################################################################################
DATADIR = path.path("datadir")

########################################################################################################################
# Création de l'archive via shutil :
########################################################################################################################

generated1 = shutil.make_archive("archive_from_shutil_make_archive__GITIGNORE__", "zip", root_dir=".", base_dir=DATADIR)
print "Generated via shutil.make_archive :"
print "\t", generated1
print ""

########################################################################################################################
# Création de l'archive via zip :
########################################################################################################################

generated2 = "archive_from_zipfile__GITIGNORE__.zip"
with zipfile.ZipFile(generated2, "w", zipfile.ZIP_DEFLATED) as dazip:
    for f in DATADIR.files():
        dazip.write(f)
print "Generated via zipfile :"
print "\t", generated2
print ""

########################################################################################################################
# On vérifie que les deux archives générées sont identiques :
########################################################################################################################

print u"Comparaison des md5 :"
hash1 = path.path(generated1).read_hexhash("md5")
hash2 = path.path(generated2).read_hexhash("md5")
print "\t", hash1
print "\t", hash2
print ""
assert(hash1 == hash2)

########################################################################################################################
# Fin du fichier.
########################################################################################################################
