#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
# set -o xtrace


cat << EOF
Dépendances :

1. installation de libosmium :
    sudo apt-get install libosmium2-dev

2. installation de ses dépendances, si besoin :
    sudo apt-get install libbz2-dev zlib1g-dev libexpat1-dev

3. disposer d'un extract osm dans /tmp/extract.osm(.pbf) :
    wget -O NOGIT_extract.osm     "https://api.openstreetmap.org/api/0.6/map?bbox=2.2903,48.7520,2.3019,48.7582"  # Antony
    wget -O NOGIT_extract.osm.pbf "https://download.bbbike.org/osm/bbbike/Bordeaux/Bordeaux.osm.pbf"
    wget -O NOGIT_extract.osm.pbf "https://download.geofabrik.de/europe/monaco-latest.osm.pbf"
EOF

extract_xml="NOGIT_extract.osm"
extract_pbf="${extract_xml}.pbf"
[ ! -f "$extract_xml" -a ! -f "${extract_pbf}" ] && echo "ERROR : missing extract  '${extract_xml}(.pbf)'" && exit 1

BUILD_DIR="_build"
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"
cmake .. && make

cd ..
if [ -f "$extract_pbf" ]
then
    "${BUILD_DIR}/osmconvert" "${extract_pbf}" /tmp/published
else
    "${BUILD_DIR}/osmconvert" "${extract_xml}" /tmp/published
fi
