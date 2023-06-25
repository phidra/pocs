#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF

TODO : description

EOF


if [ ! -d NOGIT_venv_poc_igraph ]
then
    python3.6 -m venv NOGIT_venv_poc_igraph
    NOGIT_venv_poc_igraph/bin/pip install -U pip
    NOGIT_venv_poc_igraph/bin/pip install python-igraph
    NOGIT_venv_poc_igraph/bin/pip install wheel  # to get rid of error message in cairocffi
    NOGIT_venv_poc_igraph/bin/pip install cairocffi  # for drawing graphs
fi

echo ""
echo "=== RUNNING"
NOGIT_venv_poc_igraph/bin/python poc_igraph.py
