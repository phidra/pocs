#!/bin/bash

set -o errexit
set -o nounset

rm -rf cmake_build_dir

# configure :
cmake -Bcmake_build_dir -H.

# build :
make -C cmake_build_dir

# run :
chmod +x cmake_build_dir/bin
cmake_build_dir/bin
