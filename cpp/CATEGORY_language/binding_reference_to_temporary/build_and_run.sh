#!/bin/bash

set -o errexit
set -o nounset
set -o xtrace

# build with conan + cmake :
BUILD_DIR="NOGIT_build"
mkdir -p "${BUILD_DIR}"
cmake -B "${BUILD_DIR}" .
make -C "${BUILD_DIR}" -j

# run :
"${BUILD_DIR}/pouet"
