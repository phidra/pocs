#!/usr/bin/env python
# -*- coding: utf-8 -*-
################################################################################
#
# Tests d'expression régulière, et de capture de motifs :
#
################################################################################

import re

test_string_list = ["IDENTIFIANT    = T01  ",
                    "IDENTIFIANT    =T02",
                    "IDENTIFIANT    : T3",
                    "IDENTIFIANT    = T3"]
id_pattern = "^IDENTIFIANT += +([^ ]+) *$"
print "Test avec le pattern : %s" % id_pattern
id_regex = re.compile(id_pattern)


for test_string in test_string_list:
    match = id_regex.match(test_string)
    if match:
        print "+++ La chaîne \"%s\" matche et capture le motif \"%s\"" % (test_string, match.group(1))
    else:
        print "--- La chaîne \"%s\" ne matche pas !" % test_string
    

################################################################################
# Fin du fichier.
################################################################################

