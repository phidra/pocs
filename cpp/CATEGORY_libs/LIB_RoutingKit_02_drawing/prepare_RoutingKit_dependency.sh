#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
ROUTING_KIT_DIR="${this_script_parent}/NOGIT_RoutingKit_repository"
ROUTING_KIT_URL="https://github.com/phidra/RoutingKit"

echo ""
if [ ! -d "$ROUTING_KIT_DIR" ]
then
    echo "=== Cloning RoutingKig into : $ROUTING_KIT_DIR"
    git clone "$ROUTING_KIT_URL" "$ROUTING_KIT_DIR"
else
    echo "=== Not cloning, already existing RoutingKit dir : $ROUTING_KIT_DIR"
fi


echo "=== Building RoutingKit :"
make -j -C "$ROUTING_KIT_DIR"
echo "=== Building RoutingKit is done, return status is $?"
echo ""
