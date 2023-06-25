#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Essai de dump d'un dictionnaire python (contenant des chaînes unicode, qui plus est).
# Apparemment, pour les chaînes unicode, il ajoute des tags indiquant le type python de ce qui est dumpé...
# Ça ne me plaît pas du tout, mais ça n'empêche apparemment pas le parsing...
# Si je veux éviter le dump des tags, je peux utiliser "safe_dump"
########################################################################################################################

import sys
import yaml # Nécessite d'installer le paquet python-yaml

to_dump = {}
to_dump[u"Prénom"] = u"James"
to_dump[u"Nom"] = u"BOND"
to_dump[u"Matricule"] = u"7"
to_dump[u"Matricule (n°)"] = 7
to_dump[u"Matricule courant"] = u"007"

conquetes = []
conquetes.append(u"Natalya SIMIONOVA")
conquetes.append(u"Denise RICHARDS")
conquetes.append(u"Famke JANSSENS")
conquetes.append(u"Carole ROUSSEAU")
to_dump[u"Conquêtes"] = conquetes
to_dump[u"Chef"] = u"M"

print yaml.dump(to_dump, encoding="utf-8")
print yaml.safe_dump(to_dump, encoding="utf-8")

with open("montest.yaml", "w") as f:
    yaml.dump(to_dump, f)

########################################################################################################################
# Fin du fichier.
########################################################################################################################
