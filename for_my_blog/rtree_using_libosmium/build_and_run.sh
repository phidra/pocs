#!/bin/bash

set -o errexit
set -o nounset
# set -o xtrace


cat << EOF
Charge un extract OSM dans un rtree, et permet de requêter le node le plus proche d'une long;lat donnée.

Dépendances :

1. installation de libosmium (testé avec la version 2.6.0-2):
    sudo apt install libosmium2-dev

2. installation de ses dépendances :
    sudo apt install libbz2-dev zlib1g-dev libexpat1-dev

3. disposer d'un extract OSM :
    wget -O NOGIT_extract.osm.pbf "https://download.bbbike.org/osm/bbbike/Bordeaux/Bordeaux.osm.pbf"

EOF

extract_xml="NOGIT_extract.osm"
extract_pbf="${extract_xml}.pbf"
[ ! -f "$extract_xml" -a ! -f "${extract_pbf}" ] && echo "ERROR : missing extract  '${extract_xml}(.pbf)'" && exit 1

BUILD_DIR="_build"
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"
conan install ..
cmake .. && make
#g++ -std=c++14 main.cpp -lbz2 -lz -lexpat -pthread -o rtree

# Place de la Victoire à Bordeaux :
REQUESTED_LONGITUDE="-0.572555"
REQUESTED_LATITUDE="44.831067"

cd ..
if [ -f "$extract_pbf" ]
then
    "${BUILD_DIR}/bin/rtree" "${extract_pbf}" $REQUESTED_LONGITUDE $REQUESTED_LATITUDE
else
    "${BUILD_DIR}/bin/rtree" "${extract_xml}" $REQUESTED_LONGITUDE $REQUESTED_LATITUDE
fi
