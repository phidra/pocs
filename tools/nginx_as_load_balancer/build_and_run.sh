#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

To run this POC :

1. in a terminal 1, launch a server with :
    ./run_server.sh 44444
2. in a terminal 2, launch ANOTHER server with :
    ./run_server.sh 55555
3. in a terminal 3, launch the load-balancer with :
    ./run_load_balancer.sh 7777
4. in a browser (or in a terminal, via curl), request the load-balancer :
    http://localhost:7777
5. checks that even if we didn't requested them directly, both backend servers (44444 and 55555) are alternatively requested (by the load balancer)

EOF
