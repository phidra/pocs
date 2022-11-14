#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
set -o xtrace

BUILD_DIR="NOGIT_build"
mkdir -p "${BUILD_DIR}"
cmake -B "${BUILD_DIR}" .
make -C "${BUILD_DIR}" -j

# run :
echo ""
"${BUILD_DIR}/pouet"
echo "EXITING"
