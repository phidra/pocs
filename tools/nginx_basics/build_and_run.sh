#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

To run this POC :

1. in a terminal, launch "run_server.sh"
2. in a browser (or in a terminal, via curl), request the launched server.

EOF
