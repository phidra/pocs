#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


>&2 cat << EOF

This POC builds a CH from input graph files, and dumps the CH in output files.

Human output is printed on stderr.

The output printed on stdout are the dumped output files (containing the CH).

EOF

set -o xtrace
this_script_parent="$(realpath "$(dirname "$0")" )"
ROUTING_KIT_DIR="${this_script_parent}/NOGIT_RoutingKit_repository"
BIN="$this_script_parent"/NOGIT_bin
LD_LIBRARY_PATH="$ROUTING_KIT_DIR"/lib "$BIN" \
   "MONACO_tails.txt" \
   "MONACO_heads.txt" \
   "MONACO_weights.txt" \
   "MONACO_osmids.txt" \
   "$this_script_parent"
