#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


venv=NOGIT_venv_poc_pyrosm
[ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"

if ! "$venv"/bin/python -c 'import pyrosm' &> /dev/null
then
    "$venv"/bin/pip install -U pip wheel
    "$venv"/bin/pip install pyrosm
    "$venv"/bin/pip install matplotlib
    "$venv"/bin/pip install python-igraph
    "$venv"/bin/pip install cairocffi
fi

echo ""
echo "=== RUNNING"
"$venv"/bin/python poc.py
