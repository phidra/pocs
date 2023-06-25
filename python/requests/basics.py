#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# POC d'utilisation de requests (pip install requests)
# cf. http://docs.python-requests.org/en/latest/user/quickstart/
########################################################################################################################

import requests

base_url = "https://www.youtube.com/watch"
get_parameters = {"v":"9bZkp7q19f0"}

print "BASE_URL       = ", base_url
print "GET_PARAMETERS = ", get_parameters

r = requests.get(base_url, params=get_parameters)

print ""
print "Response :"
print "\tr.status_code = ", r.status_code
print "\tr.url         = ", r.url
print "\tr.encoding    = ", r.encoding
print "\ttext[0:50]    = ", r.text[0:50]

########################################################################################################################
# Fin du fichier
########################################################################################################################
