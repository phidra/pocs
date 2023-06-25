#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

DESCRIPTION = this is a full integration of pyrosm+osmnx and RoutingKit :

Basically, it computes a shortest path using RoutingKit and displays node ranks on a bbox around the origin.

EOF

this_script_parent="$(realpath "$(dirname "$0")" )"


# on my laptop, python3 links to python3.5, but I also have python3.6
# on my desktop PC, python3 links to python3.8
if /usr/bin/env python3.6 --version &> /dev/null
then
    echo "Running with python3.6"
    venv="${this_script_parent}/NOGIT_venv_poc_igraph_3.6"
    [ ! -d "$venv" ] && /usr/bin/env python3.6 -m venv  "$venv"
else
    echo "Running with python3"
    venv="${this_script_parent}/NOGIT_venv_poc_igraph_3.x"
    [ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"
fi

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
"$venv"/bin/python poc.py
