#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

rm -rf meson_build_dir

echo ""
echo "=============== CONFIGURE BEGIN"
meson setup meson_build_dir/ .
echo "=============== CONFIGURE END"
echo ""


echo ""
echo "=============== BUILD BEGIN"
meson compile -C meson_build_dir/
echo "=============== BUILD END"
echo ""

echo ""
echo "=============== RUN BEGIN"
meson_build_dir/launcher
echo "=============== RUN END"
echo ""

echo ""
echo "=============== COMMANDES ninja : BEGIN"
cd meson_build_dir
ninja -t clean &> /dev/null
ninja -v
echo "=============== COMMANDES ninja : END"
echo ""
