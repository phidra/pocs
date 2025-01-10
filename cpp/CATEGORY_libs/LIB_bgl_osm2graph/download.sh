#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

# this script downloads monaco OSM extract if not already downloaded

this_script_parent="$(realpath "$(dirname "$0")" )"
DATA_DIR="${this_script_parent}/DATA/"
MONACO_EXTRACT="${DATA_DIR}/NOGIT_monaco.osm.pbf"
MONACO_URL="https://download.geofabrik.de/europe/monaco-latest.osm.pbf"


# partial :
if [ ! -f "${MONACO_EXTRACT}" ]
then
    echo "About to download MONACO extract from URL : ${MONACO_URL}"
    wget -O "${MONACO_EXTRACT}" "${MONACO_URL}"
else
    echo "Not downloading MONACO extract, already existing : ${MONACO_EXTRACT}"
fi
