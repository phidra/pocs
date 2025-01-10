#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"


INPUT_OSM="${this_script_parent}/NOGIT_monaco.osm.pbf"
if [ ! -e "$INPUT_OSM" ]
then
    echo "=== ERROR : missing OSM file : $INPUT_OSM"
    exit
else
    echo "=== Using OSM file : $INPUT_OSM"
fi
OUTPUT_SVG="${this_script_parent}/NOGIT_monaco.svg"

SOURCE_LAT=43.7325457
SOURCE_LNG=7.4188882
SOURCE_URL="https://www.openstreetmap.org/node/1866422978"
TARGET_LAT=43.7478497
TARGET_LNG=7.4321917
TARGET_URL="https://www.openstreetmap.org/node/1868736741"
echo "=== Using SOURCE = $SOURCE_URL"
echo "=== Using TARGET = $TARGET_URL"


this_script_parent="$(realpath "$(dirname "$0")" )"
ROUTING_KIT_DIR="${this_script_parent}/NOGIT_RoutingKit_repository"

echo ""
if [ ! -d "$ROUTING_KIT_DIR/build" ]
then
    echo "=== ERROR : missing RoutingKit dependency !"
    echo "=== Please run :"
    echo "===     ./prepare_RoutingKit_dependency.sh"
    exit
else
    echo "=== Using RoutingKit dependency in dir : $ROUTING_KIT_DIR"
fi


echo ""
echo "=== Building"
BIN="$this_script_parent"/NOGIT_bin
g++ -I"$ROUTING_KIT_DIR"/include -L"$ROUTING_KIT_DIR"/lib -std=c++11 main.cpp -o "$BIN" -lroutingkit

echo ""
echo "=== Running"
set -o xtrace
LD_LIBRARY_PATH="$ROUTING_KIT_DIR"/lib "$BIN" "$INPUT_OSM"  "$OUTPUT_SVG"

# to ease consultation :
cp "$OUTPUT_SVG" /tmp
