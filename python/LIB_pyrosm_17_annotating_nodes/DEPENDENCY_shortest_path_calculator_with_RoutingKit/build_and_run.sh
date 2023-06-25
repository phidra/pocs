#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF

This POC build a binary that can computes shortest paths.

This binary is called in another POC by a python code.

EOF

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
BIN="$this_script_parent"/NOGIT_shortest_path_calculator
g++ -I"$ROUTING_KIT_DIR"/include -L"$ROUTING_KIT_DIR"/lib -std=c++17 main.cpp -o "$BIN" -lroutingkit


echo ""
echo "=== Running"
set -o xtrace

# these are just values that work with a manual POC, you HAVE to adapt these values :
NUM_NODES=2223
SOURCE=2150
TARGET=2030
LD_LIBRARY_PATH="$ROUTING_KIT_DIR"/lib "$BIN" \
   "/tmp/NOGIT_tails.txt" \
   "/tmp/NOGIT_heads.txt" \
   "/tmp/NOGIT_weights.txt" \
   "$NUM_NODES" \
   "$SOURCE" \
   "$TARGET"
