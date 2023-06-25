#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

DEFAULT_OUTPUT_DIR="/tmp/propagation30/"

echo ""
echo "USAGE:      $0  <output-dir> (optional, default=${DEFAULT_OUTPUT_DIR})"
echo "EXAMPLE:    $0"
echo "EXAMPLE:    $0  /tmp/mysuperdir"

echo ""

cat << EOF

DESCRIPTION = plot dijkstra propagation around a specific node to illustrate

(GLIFOV project is the project that handles graph / CH / drawing related things)

EOF

OUTPUT_DIR="${1:-"${DEFAULT_OUTPUT_DIR}"}"
echo "Using OUTPUT_DIR = $OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"
echo ""

this_script_parent="$(realpath "$(dirname "$0")" )"


# on my laptop, python3 links to python3.5, but I also have python3.6
# on my desktop PC, python3 links to python3.8
if /usr/bin/env python3.6 --version &> /dev/null
then
    echo "Running with python3.6"
    venv="${this_script_parent}/NOGIT_venv_poc"
    [ ! -d "$venv" ] && /usr/bin/env python3.6 -m venv  "$venv"
else
    echo "Running with python3"
    venv="${this_script_parent}/NOGIT_venv_poc"
    [ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"
fi

PATH_TO_GLIFOV="$(realpath "${this_script_parent}/../PROJECT_glifov" )"
echo "INSTALLING GLIFOV FROM : $PATH_TO_GLIFOV"

if ! "$venv"/bin/python -c 'import pyrosm' &> /dev/null
then
    "$venv"/bin/pip install -U pip wheel
    "$venv"/bin/pip install "$PATH_TO_GLIFOV"
fi


# downloads monaco OSM extract if not already downloaded :
MONACO_EXTRACT="${this_script_parent}/NOGIT_monaco.osm.pbf"
MONACO_URL="https://download.geofabrik.de/europe/monaco-latest.osm.pbf"
if [ ! -f "${MONACO_EXTRACT}" ]
then
    echo "About to download MONACO extract from URL : ${MONACO_URL}"
    wget -O "${MONACO_EXTRACT}" "${MONACO_URL}"
else
    echo "Not downloading MONACO extract, already existing : ${MONACO_EXTRACT}"
fi


echo ""
echo "=== RUNNING"
MONACO_POLYGON="${this_script_parent}/monaco_polygon.geojson"
/usr/bin/time -p "$venv"/bin/python poc.py "$MONACO_EXTRACT" "$MONACO_POLYGON" "$OUTPUT_DIR"
