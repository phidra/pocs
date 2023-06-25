#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# pypoc de fonctionnement d'unidecode, convertissant les caractères unicode en une chaîne ascii équivalente.
#
# Par exemple :
#        "ü"    sera converti en   "u"
#        "北"   sera converti en   "Bei"
#
# Les symboles sont convertis aussi :
#        "→"    sera converti en   "-"
#        "…"    sera converti en   "..."
#
# Certains caractères non-convertissables (ou non-connus d'unidecode ?) sont convertis en "[?]"
#        "⌚"    sera converti en   "[?]"
#        "♿"    sera converti en   "[?]"
#
# Cette pypoc démontre aussi le moyen d'utiliser unidecode pour convertir n'importe quelle chaîne en une chaîne alphanumérique.
#
########################################################################################################################

import unidecode

input_strings = []
input_strings.append(u"ıöüç")
input_strings.append(u"avec espace")
input_strings.append(u"avec+caractères+spéciaux ⌚ !")

# Notons que le code produit une bytestring en sortie (et non un unicode) :
print "L'entrée est une chaîne unicode, tandis que la sortie est une bytestring :"
instr = input_strings[0]
out = unidecode.unidecode(instr)
print u"\"{input}\" ({intype})   →   \"{out}\"  ({outtype})".format(input=instr, intype=type(instr), out=out, outtype=type(out)).encode("utf-8")
print ""
print ""

########################################################################################################################

# Utilisation = transformer n'importe quelle chaîne en une chaîne alphanumérique :

def print_alphanumed(a):
    b = unidecode.unidecode(a)
    c = "".join(x for x in b if x.isalnum())
    print u"\"{unicode_input}\"   →   \"{ascii}\"   →   \"{final}\"".format(unicode_input=a, ascii=b, final=c).encode("utf-8")

for instr in input_strings:
    print_alphanumed(instr)

########################################################################################################################
# Fin du fichier.
########################################################################################################################
