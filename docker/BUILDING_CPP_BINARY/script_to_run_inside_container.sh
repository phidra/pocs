#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

g++ --version
g++ -std=c++17 /tmp/mounted/main.cpp -o /tmp/bin
/tmp/bin

echo ""
ls -lh /tmp/mounted/NOGIT_ULTRA

cd /tmp/mounted/NOGIT_ULTRA


# /bin/bash
# echo "ICI 1"
# ./STEP3_run_server.sh
# echo "FIN"
