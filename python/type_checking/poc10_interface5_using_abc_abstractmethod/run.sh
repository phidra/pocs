#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
python3 "${this_script_parent}/poc.py"

echo ""
echo "Running mypy :"
mypy "${this_script_parent}/poc.py" "${this_script_parent}/library.py"
