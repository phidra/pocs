#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF

DESCRIPTION = compute shotest path

EOF


# on my laptop, python3 links to python3.5, but I also have python3.6
# on my desktop PC, python3 links to python3.8
if /usr/bin/env python3.6 --version &> /dev/null
then
    echo "Running with python3.6"
    venv=NOGIT_venv_poc_igraph_3.6
    [ ! -d NOGIT_venv_poc_igraph_3.6 ] && /usr/bin/env python3.6 -m venv  "$venv"
else
    echo "Running with python3"
    venv=NOGIT_venv_poc_igraph_3.x
    [ ! -d NOGIT_venv_poc_igraph_3.x ] && /usr/bin/env python3 -m venv  "$venv"
fi


"$venv"/bin/pip install -U pip
"$venv"/bin/pip install python-igraph
"$venv"/bin/pip install wheel  # to get rid of error message in cairocffi
"$venv"/bin/pip install cairocffi  # for drawing graphs

echo ""
echo "=== RUNNING"
"$venv"/bin/python poc_igraph.py
