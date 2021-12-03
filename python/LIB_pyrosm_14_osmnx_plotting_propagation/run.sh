#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

DEFAULT_OUTPUT_DIR="/tmp/propagation/"

echo ""
echo "USAGE:      $0  <output-dir> (optional, default=${DEFAULT_OUTPUT_DIR})"
echo "EXAMPLE:    $0"
echo "EXAMPLE:    $0  /tmp/mysuperdir"
echo ""

OUTPUT_DIR="${1:-"${DEFAULT_OUTPUT_DIR}"}"
echo "Using OUTPUT_DIR = $OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"
echo ""

venv=NOGIT_venv_poc_pyrosm
[ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"

if ! "$venv"/bin/python -c 'import pyrosm' &> /dev/null
then
    "$venv"/bin/pip install -U pip wheel
    "$venv"/bin/pip install pyrosm
    "$venv"/bin/pip install matplotlib
    "$venv"/bin/pip install cairocffi
    "$venv"/bin/pip install networkx
    "$venv"/bin/pip install osmnx
fi

# downloads monaco OSM extract if not already downloaded :
this_script_parent="$(realpath "$(dirname "$0")" )"
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
/usr/bin/time -p "$venv"/bin/python poc.py "$OUTPUT_DIR"
