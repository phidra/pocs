#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# POC d'utilisation de mimetypes pour deviner le type de documents selon leur extension.
########################################################################################################################

import mimetypes

to_guess = ("pouet.doc",
            "pouet.pdf",
            "pouet.xls",
            "pouet.txt",
            "pouet.jpg",
            "pouet.png",
            "pouet.ppt",
            "pouet.odt",
            "pouet.zip",
            "pouet.rar",
            )

for i in to_guess:
    print "[{}]  →  \"{}\"".format(i, mimetypes.guess_type(i, False)[0])

########################################################################################################################
#
########################################################################################################################
