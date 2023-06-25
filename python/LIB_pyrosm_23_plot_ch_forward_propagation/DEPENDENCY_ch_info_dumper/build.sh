#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
ROUTING_KIT_DIR="${this_script_parent}/NOGIT_RoutingKit_repository"

>&2 echo ""
if [ ! -d "$ROUTING_KIT_DIR/build" ]
then
    >&2 echo "=== ERROR : missing RoutingKit dependency !"
    >&2 echo "=== Please run :"
    >&2 echo "===     ./prepare_RoutingKit_dependency.sh"
    exit 1
else
    >&2 echo "=== Using RoutingKit dependency in dir : $ROUTING_KIT_DIR"
fi


>&2 echo ""
>&2 echo "=== Building"
BIN="$this_script_parent"/NOGIT_bin
g++ -I"$ROUTING_KIT_DIR"/include -L"$ROUTING_KIT_DIR"/lib -std=c++17 main.cpp -o "$BIN" -lroutingkit
