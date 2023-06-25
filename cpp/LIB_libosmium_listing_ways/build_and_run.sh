#!/bin/bash

set -o errexit
set -o nounset
# set -o xtrace


cat << EOF
Cette POC itère sur chaque way et affiche ses nodes et ses tags.


Dépendances :

1. installation de libosmium :
    sudo apt-get install libosmium2-dev

2. installation de ses dépendances :
    sudo apt-get install libbz2-dev zlib1g-dev libexpat1-dev

3. disposer d'un extract osm dans /tmp/extract.osm(.pbf) :
    wget -O NOGIT_extract.osm     "https://api.openstreetmap.org/api/0.6/map?bbox=2.2903,48.7520,2.3019,48.7582"  # Antony
    wget -O NOGIT_extract.osm.pbf "https://download.bbbike.org/osm/bbbike/Bordeaux/Bordeaux.osm.pbf"


Je n'ai pas réussi à suivre les recommandations d'utiliation de la doc libosmium (mais la POC marche tout de même ^^) :
    https://osmcode.org/libosmium/manual.html#using-libosmium-in-your-own-projects

EOF

extract_xml="NOGIT_extract.osm"
extract_pbf="${extract_xml}.pbf"
[ ! -f "$extract_xml" -a ! -f "${extract_pbf}" ] && echo "ERROR : missing extract  '${extract_xml}(.pbf)'" && exit 1

BUILD_DIR="_build"
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"
cmake .. && make
#g++ -std=c++14 main.cpp -lbz2 -lz -lexpat -pthread -o poc_libosmium

cd ..
if [ -f "$extract_pbf" ]
then
    "${BUILD_DIR}/poc_libosmium" "${extract_pbf}"
else
    "${BUILD_DIR}/poc_libosmium" "${extract_xml}"
fi
