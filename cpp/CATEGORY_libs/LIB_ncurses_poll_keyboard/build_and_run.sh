#!/bin/bash

set -o errexit
set -o nounset

BUILD_DIR="_build"

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"
cmake ..
make -j4
cd ..
"${BUILD_DIR}/poll_keyboard"
