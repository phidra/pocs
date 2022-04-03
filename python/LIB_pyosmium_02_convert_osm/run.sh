#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"


venv=NOGIT_venv_poc_osmium
[ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"

if ! "$venv"/bin/python -c 'import osmium' &> /dev/null
then
    "$venv"/bin/pip install -U pip
    "$venv"/bin/pip install osmium
fi

echo ""
echo "=== RUNNING"
INPUT_OSM="${this_script_parent}/DATA_single_edge.osm"
OUTPUT_OSMPBF="${this_script_parent}/NOGIT_converted1.osm.pbf"
"$venv"/bin/python poc.py  "$INPUT_OSM"  "$OUTPUT_OSMPBF"


echo ""
echo "=== CHECKING"
osmium cat --overwrite "${this_script_parent}/DATA_single_edge.osm" -o "${this_script_parent}/NOGIT_converted2.osm.pbf"

# osm → osm.pbf → osm should give same files :
osmium cat --overwrite "${this_script_parent}/NOGIT_converted1.osm.pbf" -o "${this_script_parent}/NOGIT_back1.osm"
osmium cat --overwrite "${this_script_parent}/NOGIT_converted2.osm.pbf" -o "${this_script_parent}/NOGIT_back2.osm"


echo "COMPARING FILES :"
echo "    ${this_script_parent}/NOGIT_back1.osm"
echo "    ${this_script_parent}/NOGIT_back2.osm"

if cmp "${this_script_parent}/NOGIT_back1.osm"  "${this_script_parent}/NOGIT_back2.osm"
then
    echo ""
    echo "OK, files are identical :-)"
else
    echo ""
    echo "ERROR : files are different !"
fi

