#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# QUESTION : si j'utilise path.move vers un répertoire n'existant pas, le créé-t-il ?
# RÉPONSE  : Non, il faut le créer avant.
########################################################################################################################

import path
import tempfile

########################################################################################################################


# Création d'un répertoire temporaire pour les tests :
tmpdir =  path.path(tempfile.mkdtemp())
print "Temporary dir = ", tmpdir

# Création d'un fichier :
f = tmpdir / "essai"
f.write_text("coucou")

# Il est NÉCESSAIRE de créer les répertoires AVANT :
dst = tmpdir / "level1" / "level2" / "level3" / "level4" 
dst.makedirs_p()

# Déplacement dans un sous-répertoire lointain (plusieurs niveaux) :
f.move(dst / f.basename())


########################################################################################################################
# Fin du fichier.
########################################################################################################################


