#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

echo ""
echo "RUNNING MYPY ON BOTH FILES :"
set +o errexit
mypy "${this_script_parent}/main_without_bug.py"
mypy "${this_script_parent}/main_with_bug.py"
