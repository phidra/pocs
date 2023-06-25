#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
# set -o xtrace


cat << EOF
Cette POC correspond à mes essais autour du parsing de ligne et de fichier en C++.

ATTENTION : il y a BEAUCOUP de notes en commentaire des fichiers sources.

NOTE : on laisse de côté les fichiers sans saut de ligne final (c'est un edge-case, cf. mon blogpost).
NOTE : on laisse de côté les fichiers dont les champs contiennent des séparateurs non-échappés.

EOF


echo ""
echo "+++ compiling parse_line :"
g++ -std=c++14 parse_line.cpp -o NOGIT_bin_parse_line

echo ""
echo "+++ running parse_line :"
./NOGIT_bin_parse_line


echo ""
echo "+++ compiling parse_file :"
g++ -std=c++14 parse_file.cpp -o NOGIT_bin_parse_file

echo ""
echo "+++ running parse_file :"
./NOGIT_bin_parse_file
