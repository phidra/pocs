#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# POC de rendu de template avec jinja2.
#
# Que d'actions compliquées à faire ! J'aurais aimé une API du style :
#
#   output_string = jinja2.render(filepath="myfile", context={"key":"value"})
#
########################################################################################################################

import path
import jinja2
import sys
import datetime
import pytz


script_path = path.path(sys.argv[0]).expand().abspath().dirname()
now = pytz.utc.localize(datetime.datetime.utcnow())

# BUG : les paths doivent être convertis en string avant d'être utilisés par jinja (sinon, NameError) :
template_path = str(script_path / "index.html.j2")

########################################################################################################################

# Que de préparations !
loader = jinja2.FileSystemLoader(searchpath="/")
env = jinja2.Environment(loader=loader)
template = env.get_template(template_path)

# Contexte :
context = {}
context["title"] = "Da MF Title !"
context["user"] = "myself"
context["datetime"] = now.astimezone(pytz.timezone("Europe/Paris"))

# Rendu :
output = template.render(context)

# Écriture :
dst = script_path / "index.html"
dst.write_text(output.encode("utf-8"))
print "OK pour la génération, maintenant, essayez d'ouvrir le template :"
print "\tfirefox index.html"

########################################################################################################################
# Fin du fichier.
########################################################################################################################
