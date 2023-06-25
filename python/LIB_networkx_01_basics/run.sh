#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF

DESCRIPTION : POC of basics operations with networkx.
The biggest difference with igraph is that you can't 'select' or 'find'
(you have to iterate over all edges/nodes and filter them)

EOF

this_script_parent="$(realpath "$(dirname "$0")" )"

# on my laptop, python3 links to python3.5, but I also have python3.6
# on my desktop PC, python3 links to python3.8
if /usr/bin/env python3.6 --version &> /dev/null
then
    echo "Running with python3.6"
    venv="${this_script_parent}/NOGIT_venv_poc_networkx_3.6"
    [ ! -d "$venv" ] && /usr/bin/env python3.6 -m venv  "$venv"
else
    echo "Running with python3"
    venv="${this_script_parent}/NOGIT_venv_poc_networkx_3.x"
    [ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"
fi

if ! "$venv"/bin/python -c 'import networkx' &> /dev/null
then
    "$venv"/bin/pip install -U pip wheel
    "$venv"/bin/pip install networkx
fi

"$venv"/bin/python -c 'import networkx' &> /dev/null

echo ""
echo "=== RUNNING"
"$venv"/bin/python poc.py
