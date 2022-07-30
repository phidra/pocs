#!/bin/bash

echo ""
echo "This POC shows that sqlite3 can be unsed to easily analyze CSV files."
echo "cf. https://antonz.org/sqlite-is-not-a-toy-database/"
echo "cf. https://www.sqlite.org/json1.html"
echo ""

# cf. https://www.sqlite.org/json1.html
#   readfile     = lit un fichier et retourne une string avec son contenu
#   json_each    = lit une string json, et itère sur son contenu (ou un sous-ensemble identifié par le json-path)
#   json_extract = lit une string json, et extrait une valeur à partir d'un json-path)

sqlite3 < mysuperscript
