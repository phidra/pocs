#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
# set -o xtrace

# note : les exemples ne sont donnés qu'avec le parser python, mais le comportement est identique avec le parser ruby.

echo ""
echo "od -c illustration1_text_eol.csv"
od -c illustration1_text_eol.csv

echo ""
echo "od -c illustration2_text_noeol.csv       <-- pas de '\n' final !"
od -c illustration2_text_noeol.csv

echo ""
echo "od -c illustration3_notext_eol.csv"
od -c illustration3_notext_eol.csv


echo ""
echo "od -c illustration4_notext_noeol.csv     <-- pas de '\n' final !"
od -c illustration4_notext_noeol.csv

echo ""
echo "./parse_csv.py illustration1_text_eol.csv illustration2_text_noeol.csv"
./parse_csv.py illustration1_text_eol.csv illustration2_text_noeol.csv

echo ""
echo "./parse_csv.py illustration3_notext_eol.csv illustration4_notext_noeol.csv"
./parse_csv.py illustration3_notext_eol.csv illustration4_notext_noeol.csv

# echo ""
# echo "./parse_csv.py test1_eol.csv test2_noeol.csv"
# ./parse_csv.py test1_eol.csv test2_noeol.csv
# echo ""
# echo "./parse_csv.py test3_empty_line_eol.csv test4_empty_line_noeol.csv"  # <-- se comporte différement
# ./parse_csv.py test3_empty_line_eol.csv test4_empty_line_noeol.csv
# echo ""
# echo "./parse_csv.py test5_singlecomma_eol.csv test6_singlecomma_noeol.csv"
# ./parse_csv.py test5_singlecomma_eol.csv test6_singlecomma_noeol.csv
# echo ""
# echo "./parse_csv.py test7_spaces_eol.csv test8_spaces_noeol.csv"
# ./parse_csv.py test7_spaces_eol.csv test8_spaces_noeol.csv


cat << EOF

Cette POC montre un comportement un poil inhomogène des parsers CSV, vis-à-vis de la dernière ligne du fichier.

Plus de détail dans le billet de blog, mais en gros, si le dernier caractère d'un fichier CSV n'est pas '\n'
alors le comportement n'est pas cohérent avec le reste du parsing.

La POC illustre ce comportement sur deux paires de fichiers.
Au sein d'une paire, les deux fichiers sont identiques, à ceci près que l'un contient un '\n' final et pas l'autre.

Pour le parser CSV de python/ruby/libreoffice, le comportement diffère selon que la dernière ligne est vide ou pas :

Tous les parsers parseront ces deux fichiers DE LA MÊME FAÇON :
    text1\ntext2\n   <-- parsé en deux lignes "text1" et "text2"
    text1\ntext2     <-- parsé en deux lignes "text1" et "text2"

Mais si 'text2' est une chaîne vide, alors tous les parers parserontces deux fichiers DIFFÉREMMENT :
    text1\n\n        <-- parsé en DEUX lignes "text1" et "" (ligne vide)
    text1\n          <-- parsé en UNE SEULE ligne "text1"
EOF

