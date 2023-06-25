#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF

DESCRIPTION = tests of grouping data

lines of stops.txt are of 2 kinds : parent and child stations

We want to regroup child stations by their parent

EOF


venv=NOGIT_venv_poc_pandas
[ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"

if ! "$venv"/bin/python -c 'import pandas' &> /dev/null
then
    "$venv"/bin/pip install -U pip wheel
    "$venv"/bin/pip install pandas
fi

echo ""
echo "=== RUNNING"
"$venv"/bin/python poc.py
