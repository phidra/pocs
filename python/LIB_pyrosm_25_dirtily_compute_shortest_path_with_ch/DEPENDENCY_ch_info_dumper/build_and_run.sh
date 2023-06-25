#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"


>&2 echo ""
>&2 echo "=== Building"
"${this_script_parent}/build.sh"


>&2 echo ""
>&2 echo "=== Running"
"${this_script_parent}/run.sh"
