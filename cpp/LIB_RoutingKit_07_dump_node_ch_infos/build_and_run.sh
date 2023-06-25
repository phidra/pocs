#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


>&2 cat << EOF

This POC builds a CH, and analyzes a given node :

    - what is its rank/rkindex/osmid
    - what are its ORIGINAL successors (i.e. head of out-edge)
    - what are its CONTRACTED successors, and by which node does the shortcut goes.

Human output is printed on stderr.

The output printed on stdout is designed to be parsed by an external program.

EOF

this_script_parent="$(realpath "$(dirname "$0")" )"
ROUTING_KIT_DIR="${this_script_parent}/NOGIT_RoutingKit_repository"

>&2 echo ""
if [ ! -d "$ROUTING_KIT_DIR/build" ]
then
    >&2 echo "=== ERROR : missing RoutingKit dependency !"
    >&2 echo "=== Please run :"
    >&2 echo "===     ./prepare_RoutingKit_dependency.sh"
    exit
else
    >&2 echo "=== Using RoutingKit dependency in dir : $ROUTING_KIT_DIR"
fi


>&2 echo ""
>&2 echo "=== Building"
BIN="$this_script_parent"/NOGIT_bin
g++ -I"$ROUTING_KIT_DIR"/include -L"$ROUTING_KIT_DIR"/lib -std=c++17 main.cpp -o "$BIN" -lroutingkit


>&2 echo ""
>&2 echo "=== Running"
set -o xtrace

# these are just values that work with a manual POC, you HAVE to adapt these values :
NODE_TO_ANALYZE=150
LD_LIBRARY_PATH="$ROUTING_KIT_DIR"/lib "$BIN" \
   "MONACO_tails.txt" \
   "MONACO_heads.txt" \
   "MONACO_weights.txt" \
   "MONACO_osmids.txt" \
   "$NODE_TO_ANALYZE"
