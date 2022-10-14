#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"


myvenv=NOGIT_venv_poc_ultradict
[ ! -d "$myvenv" ] && /usr/bin/env python3 -m venv  "$myvenv"

if ! "$myvenv"/bin/python -c 'import UltraDict ; import psutil' &> /dev/null
then
    "$myvenv"/bin/pip install -U pip wheel
    "$myvenv"/bin/pip install UltraDict
    "$myvenv"/bin/pip install psutil
fi

echo ""
echo "=== RUNNING"
"$myvenv"/bin/python "${this_script_parent}/poc.py"
