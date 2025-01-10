#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
ROUTING_KIT_DIR="${this_script_parent}/RoutingKit_repository_CUSTOMIZED"


# Info on original version of RoutingKit (modified by this POC) :
cat << EOF
This POC uses a slightly modified version of RoutingKit, that adds cout to see what happens.
This modified version is derived from :
    https://github.com/phidra/RoutingKit/tree/a0776b234ac6e86d4255952ef60a6a9bf8d88f02

Last commit was :
    commit a0776b234ac6e86d4255952ef60a6a9bf8d88f02 (HEAD -> master, origin/master, origin/HEAD)
    Merge: 613b725 627ae3c
    Author: ben-strasser <ben-strasser@users.noreply.github.com>
    Date:   Wed Mar 31 14:55:49 2021 +0200
        Merge pull request #95 from MojtabaHs/master
            Fix some spellings in the docs.

To date, the only modification is adding couts to "src/contraction_hierarchy.cpp"

Compared to POC 04, this POC uses a directed graph instead of a bidirectional one.

EOF


# Building RoutingKit :
make -j -C "$ROUTING_KIT_DIR"

# Building POC :
echo ""
echo "=== Building"
BIN="$this_script_parent"/NOGIT_bin
g++ -I"$ROUTING_KIT_DIR"/include -L"$ROUTING_KIT_DIR"/lib -std=c++17 main.cpp -o "$BIN" -lroutingkit

echo ""
echo "=== Running"
set -o xtrace
LD_LIBRARY_PATH="$ROUTING_KIT_DIR"/lib "$BIN"
