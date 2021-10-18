#!/bin/bash

set -o errexit
set -o nounset
set -o xtrace

# build with conan + cmake :
BUILD_DIR="_build"
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"
conan install ..
cmake ..
make -j

# run :
cd ..
"${BUILD_DIR}/bin/echo" 8888
