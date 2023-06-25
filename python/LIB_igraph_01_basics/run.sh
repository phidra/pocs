#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF

TODO : description

EOF


if [ ! -d NOGIT_venv_poc_igraph ]
then
    python3 -m venv NOGIT_venv_poc_igraph
    NOGIT_venv_poc_igraph/bin/pip install python-igraph
fi

echo ""
echo "=== RUNNING"
NOGIT_venv_poc_igraph/bin/python poc_igraph.py
