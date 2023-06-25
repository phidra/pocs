#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Tests d'utilisation du module csv.
#
# cf. https://docs.python.org/2/library/csv.html#csv-examples
#
# On montre deux types de parsing :
#   - le parsing des lignes d'un fichier CSV encodé en UTF-8 (séparateur = ";")
#   - le parsing d'une string unicode dont les champs sont séparés par des virgules
########################################################################################################################

import sys
import subprocess
import csv

########################################################################################################################
# CONFIG :
########################################################################################################################

try:
    csvfile = sys.argv[1]
except IndexError:
    print ""
    print "ERROR - missing arg (csvfile)"
    print ""
    print "\tusage = {} csvfile".format(sys.argv[0])
    print ""
    sys.exit(10)

########################################################################################################################
# Lecture et decodage :
########################################################################################################################

with open(csvfile, 'rb') as f:
    reader = csv.reader(f, delimiter=';', lineterminator="\r\n", quoting=csv.QUOTE_NONE)
    for row in reader:
        urow = [unicode(cell, 'utf-8') for cell in row]
        uklasses = urow[-1]

        # NOTE : pour parser directement une string, il suffit d'en faire un itérable :
        parsed_klasses = csv.reader((uklasses, ), delimiter=",", quoting=csv.QUOTE_NONE)
        klasses = parsed_klasses.next()

        print u"[LIGNE {num:03}]    ".format(num=reader.line_num) + u" / ".join(urow[:-1] + [u"+".join(klasses), ])

########################################################################################################################
# Fin du fichier.
########################################################################################################################
