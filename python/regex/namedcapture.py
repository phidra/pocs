#!/usr/bin/env python
# -*- coding: utf-8 -*-
################################################################################
#
# Tests d'expression régulière, et de capture de motifs nommés.
#
################################################################################

import re

INPUTS = (
        "Luke est petit",
        "Leia est jolie",
        "Obiwan est badass",
        )

REGEX = "^(?P<name>[^ ]*) est (?P<adjectif>[^ ]*)$"

for input_ in INPUTS:
    match = re.match(REGEX, input_)
    if not match:
        raise ValueError("'{}' should have matched !".format(input_))
    print ""
    print "NAME = ", match.group("name")
    print "ADJ  = ", match.group("adjectif")
