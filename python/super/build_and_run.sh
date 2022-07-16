#!/bin/bash

set -o errexit
set -o nounset


this_script_parent="$(realpath "$(dirname "$0")" )"

echo """
This POC shows edge-cases of super, supporting unstructured notes.
"""

python3 "${this_script_parent}/poc.py"
