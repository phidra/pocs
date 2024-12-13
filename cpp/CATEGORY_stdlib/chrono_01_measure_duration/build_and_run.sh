#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
echo "parent = $this_script_parent" > /dev/null

rm -rf NOGIT_build
CXX=clang++-15 meson setup NOGIT_build/ .
CXX=clang++-15 meson compile  -C NOGIT_build/
NOGIT_build/bin
