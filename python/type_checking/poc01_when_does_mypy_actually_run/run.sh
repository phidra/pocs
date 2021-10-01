#!/bin/bash

set +o errexit  # some commands actually fail below, this is normal
set -o nounset
set -o pipefail


this_script_parent="$(realpath "$(dirname "$0")" )"

echo ""
echo "===== STEP 1 = showing that the execution actually fails :"
python3 "${this_script_parent}/poc.py"


echo ""
echo "===== STEP 2 = showing that mypy doesn't detect the bug :"
mypy "${this_script_parent}/poc.py"
echo "MYPY RETURNCODE = $?"


echo ""
echo "===== STEP 3 = showing that with --check-untyped-defs, mypy DOES detect the bug :"
mypy --check-untyped-defs "${this_script_parent}/poc.py"
echo "MYPY RETURNCODE = $?"


echo ""
echo "===== STEP 4 = alternative to 'force' mypy to detect the bug = annotate the function :"
mypy "${this_script_parent}/poc_with_annotated_main.py"
echo "MYPY RETURNCODE = $?"
