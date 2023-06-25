#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

To run this POC :

1. in a terminal 1, launch "run_server.sh"
2. in a terminal 2, launch "run_reverse_proxy.sh"
3. in a browser (or in a terminal, via curl), request the reverse-proxy.
4. checks that even if we didn't requested it directly, the server was requested (by the reverse-proxy)

EOF
