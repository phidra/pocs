#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
BUILD_DIR="$this_script_parent/src/_build"
CMAKE_ROOT_DIR="$this_script_parent/src"
echo "BUILD_DIR=$BUILD_DIR"
echo "CMAKE_ROOT_DIR=$CMAKE_ROOT_DIR"


# configure :
pushd "$CMAKE_ROOT_DIR"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
conan install --install-folder="$BUILD_DIR" .
cmake -B"$BUILD_DIR" -H"$CMAKE_ROOT_DIR"
popd

# build :
make -j -C "$BUILD_DIR"

# run :
set -o xtrace
src/_build/bin/convexhull
