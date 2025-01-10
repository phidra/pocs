#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
# set -o xtrace


cat << EOF
Dépendances au build :

1. installation de libosmium (et ses dépendances, si besoin) :
    sudo apt-get install libosmium2-dev
    sudo apt-get install libbz2-dev zlib1g-dev libexpat1-dev

2. installation de conan (pour rapidjson et boost) :
    pipx install conan
EOF

this_script_parent="$(realpath "$(dirname "$0")" )"
BUILD_DIR="$this_script_parent/src/_build"
CMAKE_ROOT_DIR="$this_script_parent/src"
echo "BUILD_DIR=$BUILD_DIR"
echo "CMAKE_ROOT_DIR=$CMAKE_ROOT_DIR"


pushd "$CMAKE_ROOT_DIR"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
conan install --install-folder="$BUILD_DIR" . --profile=conanprofile.txt
CC=clang CXX=clang++ cmake -B"$BUILD_DIR" -H"$CMAKE_ROOT_DIR"
popd
