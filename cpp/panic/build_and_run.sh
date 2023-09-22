#!/bin/bash

set -o errexit
set -o nounset

echo ""
echo "==================================================================================="
echo "WARNING : cette POC ne fonctionne qu'avec un compilo récent, supportant std::format"
echo "Elle compile avec g++ 13, mais pas avec g++ 12"
echo "Elle ne compile ni avec clang 16 ni avec clang 17"
echo "==================================================================================="
echo ""

set -o xtrace

# build with conan + cmake :
BUILD_DIR="NOGIT_build"
mkdir -p "${BUILD_DIR}"
cmake -B "${BUILD_DIR}" .
make -C "${BUILD_DIR}" -j

# run :
"${BUILD_DIR}/pouet"
