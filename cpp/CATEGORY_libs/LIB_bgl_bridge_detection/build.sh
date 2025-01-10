#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
# set -o xtrace


this_script_parent="$(realpath "$(dirname "$0")" )"
BUILD_DIR="$this_script_parent/src/_build"
echo "BUILD_DIR=$BUILD_DIR"

make -j -C "$BUILD_DIR"
