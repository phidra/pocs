#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


venv=NOGIT_venv_poc_osmium
[ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"

if ! "$venv"/bin/python -c 'import osmium' &> /dev/null
then
    "$venv"/bin/pip install -U pip
    "$venv"/bin/pip install osmium
fi

echo ""
echo "=== RUNNING"
"$venv"/bin/python poc.py
