#!/bin/bash

set -o errexit
set -o nounset
set -o xtrace

this_script_parent="$(realpath "$(dirname "$0")" )"
echo "parent = $this_script_parent" > /dev/null


# build with cmake :
BUILD_DIR="NOGIT_build"
mkdir -p "${BUILD_DIR}"
cmake -B "${BUILD_DIR}" .
make -C "${BUILD_DIR}" -j

# run :
"${BUILD_DIR}/pouet"
