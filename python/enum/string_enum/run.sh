#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

echo ""
echo "RUNNING THE MAIN :"
python3 "${this_script_parent}/main.py"
