#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# POC de démonstration sur l'utilisation de la librairie (installée avec pip) lipsum.
#
# En gros basiquement, on fait :
#
#       import lipsum
#       g = lipsum.Generator()
#       g.generate_sentence()
#       g.generate_paragraph()
#
########################################################################################################################

import lipsum


def separate(msg):
    print u"\n" +u"\n" +  120*u"=" + u"\n" + u"=== {}\n".format(msg) + 120*u"=" + u"\n"

g = lipsum.Generator()

separate("SENTENCE")
print g.generate_sentence()


separate("PARAGRAPH")
print g.generate_paragraph()


separate("PARAGRAPH starting with lorem")
print g.generate_paragraph(start_with_lorem=True)


########################################################################################################################
# Fin du fichier.
########################################################################################################################


