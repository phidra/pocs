#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Syntaxes basiques d'utilisation de fichiers / répertoires temporaires :
########################################################################################################################

import path
import tempfile
import datetime

########################################################################################################################
# Répertoires (wrappés dans un path.path pour plus de simplicité) :
########################################################################################################################

# Répertoire temporaire anonyme :
tmpdir1 =  path.path(tempfile.mkdtemp())
print "tmpdir1 = ", tmpdir1

# Répertoire temporaire avec préfixe :
tmpdir2 =  path.path(tempfile.mkdtemp(prefix="mysuperprefix_"))
print "tmpdir2 = ", tmpdir2

# Répertoire temporaire avec préfixe daté :
now = datetime.datetime.now().strftime("%Y%B%d_%Hh%Mm%Ss")
tmpdir3 =  path.path(tempfile.mkdtemp(prefix="dateprefix_{}_".format(now)))
print "tmpdir3 = ", tmpdir3

# NOTE : utiliser un répertoire temporaire dans un ContextManager n'existe qu'en python 3 (cf. http://stackoverflow.com/a/19299884)

########################################################################################################################
# Fichiers :
########################################################################################################################

with tempfile.NamedTemporaryFile(suffix=u".txt", prefix="MYTEMPFILE_", delete=False) as tmpfile1:
    tmpfile1.write("Coucou")
    print "tmpfile1 = ", tmpfile1.name
    # NOTE : avec delete = True, le fichier est détruit dès qu'il est fermé.

########################################################################################################################
# Fin du fichier.
########################################################################################################################


