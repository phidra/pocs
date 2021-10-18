#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


venv=NOGIT_venv_poc_shapely_3.x
[ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"

if ! "$venv"/bin/python -c 'import shapely' &> /dev/null
then
    "$venv"/bin/pip install -U pip wheel
    "$venv"/bin/pip install shapely
fi

echo ""
echo "=== RUNNING"
"$venv"/bin/python poc.py
