#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# BESOIN 1 = à partir d'un host et d'une série de paramètres GET, je veux forger une URL robuste et exploitable.
#      notamment, je veux que les caractères spéciaux des paramètres GET soient correctement gérés
#      pour encoder les caractères spéciaux des paramètres, on utilise urllib.urlencode
#      les paramètres doivent toutefois être encodés : urllib.urlencode n'accepte que des bytestring en entrée
#      pour assembler l'url, on PEUT utiliser urlparse
#      mais ce n'est pas obligatoire, on peut simplement accoler l'host et les paramètres encodés
#
# BESOIN 2 = quelle différence entre urllib.urlencode et urllib.quote_plus ?
#      urllib.urlencode est simplement l'application de urllib.quote_plus à tous clé+valeur des paramètres GET

# BESOIN 3 = comment forger une URL à la main ?
#      L'objet urlparse.ParseResult a une méthode geturl qui produit l'URL finale.
#      C'est un peu lourd -> c'est sans doute aussi simple de faire une simple string "{host}&{params}"
#
# BESOIN 4 = mise en application complète : comment modifier un paramètre GET dans une URL donnée ?
#      on utilise urlparse.urlparse sur l'URL pour la splitter en récupérant un objet ParseResult
#      derrière, on splite les paramètres GET avec urlparse.parse_qs (ou urlparse.parse_qsl)
#      on est alors libre de modifier les paramètres comme on l'entend, puis de reconstruire l'URL
#
########################################################################################################################

from urllib import urlencode, quote_plus
import urlparse

# BESOIN 1 = encoder les caractères spéciaux des paramètres GET :
print ""
print u"Comment encoder des caractères spéciaux dans les paramètres GET ?"
host = "http://fr.ouibus.com/fr/calendrier-des-prix/"
params = {
        "SRC": "Paris",
        "DST": u"Mont Saint-Hélène".encode("utf-8"), # les paramètres doivent être encodés (bytestring attendus)
        "DATE": "2017/05/31",
        }

# SOLUTION = urlencode pour les caractères spéciaux des paramètres + on forge l'URL à la main :
url1 = "{}?{}".format(host, urlencode(params))
print url1

########################################################################################################################

# BESOIN 2 = quelle différence entre urlencode et quote_plus ?
print ""
print u"Quelle différence entre urlencode et quote_plus ?"
params_as_list = ( # on utilise une list plutôt qu'un dict pour conserver le même ordre, mais le principe est inchangé
        ("SRC", "Paris"),
        ("DST", u"Mont Saint-Hélène".encode("utf-8")),
        ("DATE", "2017/05/31"),
        )

# RÉPONSE = urlencode est équivalent à l'application de quote_plus à tous les clés+valeurs :
with_urlencode = urlencode(params_as_list)
with_quoteplus = "&".join(["{}={}".format(quote_plus(key), quote_plus(value)) for key, value in params_as_list])
print "with_urlencode = ", with_urlencode
print "with_quoteplus = ", with_quoteplus

# ########################################################################################################################

# BESOIN 3 = comment forger une URL à la main ?
print ""
print u"Comment forger une URL à la main ?"
parse_object = urlparse.ParseResult("scheme", "netloc", "path", "params", "query", "fragment")
print "dummy ParseResult --> ", parse_object.geturl()


# SOLUTION = utiliser la méthode geturl de urlparse.ParseResult (éventuellement construit à la main) :
wikiparse= urlparse.ParseResult(
        scheme="https",
        netloc="fr.wikipedia.org",
        params="",
        path="/my/super/page/",
        query="KEY1=val1&KEY2=val2",
        fragment="title1")
print "Exemple complet :     ", wikiparse.geturl()

# ########################################################################################################################

# BESOIN 4 = mise en application complète : comment modifier un paramètre GET dans une URL donnée ?
print ""
print u"Comment modifier un paramètre GET dans une URL donnée ?"

input_url = url1 # on reprend celle de l'exemple 1
print "INPUT  = ", input_url


# On splitte pour récupérer les paramètres GET :
parsed = urlparse.urlparse(input_url)
params_as_list = urlparse.parse_qsl(parsed.query)

# On modifie un paramètre GET :
modified_params = []
for key, val in params_as_list:
    if key == "DST":
        val = u"Les Ponts de Cé".encode("utf-8")
    modified_params.append((key, val))

# On reconstruit l'URL :
modified_parsed = parsed._replace(query=urlencode(modified_params))
output_url = modified_parsed.geturl()
print "OUTPUT = ", output_url
