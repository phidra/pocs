#!/usr/bin/env python
# NO ENCODING HERE !
########################################################################################################################
#
# Cette POC en plusieurs fichiers montre les impacts respectifs sur des bytestrings de :
#     l'encodage python déclaré en tête de fichier
#     l'encodage du contenu du fichier (défini par l'éditeur de texte au moment de son enregistrement)
#
# Pour cela, le même code est utilisé avec plusieurs encodages déclarés différents en tête de fichiers.
#
# %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# %%%%%%%%%%%%% EN RÉSUMÉ %%%%%%%%%%%%%
# %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# La POC montre que :
#     Lorsqu'on définit une bytestring, l'encodage python déclaré n'a pas d'impact sur la suite d'octets finale.
#     En effet, la bytestring se voit affecté une suite d'octets qui correspond au contenu réel (plutôt qu'affiché).
#     Et le lien entre le contenu réel et affiché ne dépend que de l'encodage du fichier, défini par l'éditeur de texte.
#     (et non de l'encodage python déclaré du fichier)
#
# Par ailleurs, pour les cas marginaux :
#     Si on utilise des caractères non-ascii dans le code-source, on DOIT OBLIGATOIREMENT déclarer un encodage python.
#         (quelque soit l'endroit où est utilisé le caractère non-ascii, y compris dans les commentaires !)
#     Si on déclare un encodage python en tête du fichier, son contenu DOIT être interprétable dans cet encodage.
#
########################################################################################################################
#
# CAS DU PRÉSENT FICHIER : il ne déclare pas d'encoding.
# C'est un cas particulier traité différemment par python : seuls les caractères ascii sont autorisés.
# Conséquemment, python lève une SyntaxError au première caractère non-ascii rencontré (même dans un commentaire).
# Ce cas n'a pas de réel rapport avec ce qui est testé par la POC.
#
########################################################################################################################

s1 = "he"
s2 = "h\xc3\xa9"  # c'est l'encodage de "hé" en UTF-8
s3 = "hé"

# On vérifie que l'encodage python déclaré en tête de fichier N'A PAS D'IMPACT sur la construction d'une bytestring :
assert(s2 is s3)

# En effet, ce que contient le fichier, ce n'est pas "hé", c'est bien la suite d'octets C3A9.
# (car l'éditeur utilisé lorsqu'on a enregistré le contenu du fichier pour "hé" utilisait l'encodage UTF-8)
# C'est donc logique que l'encodage python déclaré n'ait pas d'impact sur le contenu de s3.

assert(len(s1) == 2)
assert(len(s2) == 3)
assert(len(s3) == 3)

print "OK"
