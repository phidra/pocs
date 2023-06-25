#!/usr/bin/env python
# -*- coding: cp1252 -*-
########################################################################################################################
#
# Cette POC en plusieurs fichiers montre les impacts respectifs sur des unicodes de :
#     l'encodage python déclaré en tête de fichier
#     l'encodage du contenu du fichier (défini par l'éditeur de texte au moment de son enregistrement)
#
# L'enjeu de la POC est de savoir ce que contient la variable suivante :
#
#     pouet = u"é"
#
# Pour cela, le même code est utilisé avec plusieurs encodages déclarés différents en tête de fichiers.
#
# %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# %%%%%%%%%%%%% EN RÉSUMÉ %%%%%%%%%%%%%
# %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# La POC montre que lorsque l'interpréteur rencontre l'instruction suivante :
#
#       pouet = u"é"
#
# Tout se passe comme s'il exécutait les instructions suivantes :
#
#       pouet = "é".decode(encodage_déclaré_en_tête_de_fichier)
#
########################################################################################################################

# La POC investigue ce qui se passe lorsque l'interpréteur recontre u"é"
# Il nous faut donc une autre façon de représenter le caractère :
ecute_unicode      = u"\xe9"      # par son point de code
ecute_utf8_bytestr = "\xc3\xa9"   # par son encodage UTF-8
assert(ecute_utf8_bytestr.decode("utf-8") == ecute_unicode) # on vérifie qu'on a bien le bon.

# On vérifie que la chaîne interprétée est bien celle attendue.
# Comme l'encodage utilisé pour le decode n'est pas UTF-8, on n'attend pas le caractère unicode du "é" :
assert(u"é" != ecute_unicode)
# On s'attend à ce que ce soit le décodage en cp1252 du contenu du fichier, soit la représentation UTF-8 du "é" :
assert(u"é" == "é".decode("cp1252"))
assert(u"é" == "\xc3\xa9".decode("cp1252"))
assert(u"é" == ecute_utf8_bytestr.decode("cp1252"))

print "OK (see file for details)"

########################################################################################################################
#
# CAS DU PRÉSENT FICHIER : il déclare l'encodage python UTF-8.
# Il se trouve que le contenu réel du fichier est encodé en UTF-8 (défini par vim lorsque je l'ai rédigé).
# Par conséquent, ce qui s'affiche à l'écran comme "é" contient la chaîne UTF-8 du caractère é (C3A9).
# Quand l'interpréteur décode ce C3A9, il utilise l'encodage python déclaré (soit CP1252 ici)...
# Il tombe donc sur Ã© (soit "\xc3\xa9".decode("cp1252")
#
########################################################################################################################
