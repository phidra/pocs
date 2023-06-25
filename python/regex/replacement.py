#!/usr/bin/env python
# -*- coding: utf-8 -*-
################################################################################
#
# Tests d'expression régulière, et de remplacement de sous-motifs.
################################################################################

import re


input_string = "Maître Corbeau sur un arbre perché, tenait dans son bec un fromage."

# Il vaut mieux bien protéger les patterns, car ils ont souvent des caractères spéciaux...
pattern_str = """Ma(î|i)tre ([^ ]*) """
pattern = re.compile(pattern_str)


################################################################################
# Fonction utilitaire = afficher un MatchObject :
################################################################################

def display_match_object(mo):
    # On peut connaître la localisation du pattern dans la chaîne d'entrée :
    pattern_location_in_input_string_start = pliis_start = mo.start(0)
    pattern_location_in_input_string_end = pliis_end = mo.end(0)

    print "Le pattern a été trouvé du caractère {begin} au {end} de la chaîne d'entrée".format(begin=pliis_start, end=pliis_end-1)
    print "Il a capturé les motifs suivants :"

    try:
        for i in range(0,10):

            # Pour chaque groupe capturé, on peut connaître le groupe et sa localisation dans la chaîne d'entrée :
            captured_group = cg = mo.group(i)
            group_location_in_input_string_start = gliis_start = mo.start(i)
            group_location_in_input_string_end = gliis_end = mo.end(i)

            output = "\t[- du caractère {begin} au {end}]  group({i}) = \"{group}\""
            print output.format(begin=gliis_start, end=gliis_end-1, i=i, group=cg)
    except IndexError:
        pass
    print ""

################################################################################
# POC 1 = capturer un motif :
################################################################################

print ""
print "================================================================================"
print "===== POC 1 = capturer un motif"
print "================================================================================"
print ""

print "input_string = \"{input_string}\"".format(input_string=input_string)
print "pattern      = \"{pattern}\"".format(pattern=pattern_str)
print ""

# Démonstration de capture de plusieurs groupes dans une regex :
match = pattern.search(input_string)
if match is not None:
    display_match_object(match)

print ""

################################################################################
# POC 2 = capturer plusieurs fois un ou des motif :
################################################################################

print ""
print "================================================================================"
print "===== POC 2 = capturer plusieurs motif"
print "================================================================================"
print ""


input_string += "\nMaitre Renard par l'odeur alléché, lui tint à peu près ce langage."
print "input_string = \"{input_string}\"".format(input_string=input_string)
print "pattern      = \"{pattern}\"".format(pattern=pattern_str)
print ""

match_objects = re.finditer(pattern, input_string)

# Démonstration de capture de plusieurs groupes dans une regex :
match = pattern.search(input_string)
for match in match_objects:
    display_match_object(match)
print ""


################################################################################
# CAS PRATIQUE : simule le renommage robuste d'un nom de fichier pour passer un numéro sur 5 chiffres.
#
# On montre notamment qu'on peut récupérer la position de la regex (ou de sous-regex
# encadrées par des parenthèses) dans la chaîne initiale.
#
################################################################################

print ""
print "================================================================================"
print "===== POC 3 = CAS PRATIQUE = renommage robuste de noms de fichiers..."
print "================================================================================"
print ""

filename1 = "MonFichierConcernantLaGuerreDe1418_140.data"
filename2 = "MonFichierConcernantLaGuerreDe1418_141.data"
filename3 = "MonFichierConcernantLaGuerreDe1418_142.data"
filename4 = "AucunLienJeSuisFilsUnique"
flist = (filename1, filename2, filename3, filename4)


# Regex à chercher = la fin du nom du fichier (en capturant le numéro de fichier) :
pattern = re.compile("""_([0-9]+).data""")

for f in flist:
    print ""
    print "Traitement de \"{}\"".format(f)
    match_object = pattern.search(f)
    if match_object is not None:
        print "\tPosition du pattern complet dans le nom de fichier  :  [{0}:{1}]".format(match_object.start(0), match_object.end(0))
        print "\tPosition du numéro dans le nom de fichier  :  [{0}:{1}]".format(match_object.start(1), match_object.end(1))
        print "\tProposition de remplacement par  :  {0}".format(f[:match_object.start(1)] + "00" + match_object.group(1) + f[match_object.end(1):])
    else:
        print "\tNe correspond pas au pattern..."




################################################################################
# Fin du fichier.
################################################################################

