#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF

TODO : description

DEPENDENCY :
sudo apt install graphviz graphviz-dev

EOF


if [ ! -d NOGIT_venv_poc_pygraphviz ]
then
    python3 -m venv NOGIT_venv_poc_pygraphviz
    NOGIT_venv_poc_pygraphviz/bin/pip install pygraphviz
fi

echo ""
echo "=== RUNNING"
NOGIT_venv_poc_pygraphviz/bin/python poc_pygraphviz.py
