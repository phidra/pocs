#!/bin/bash

set -o errexit
set -o nounset

rm -rf cmake_build_dir

echo ""
echo "=============== CONFIGURE (cmake) BEGIN"
cmake -Bcmake_build_dir -Hsrc
echo "=============== CONFIGURE (cmake) END"
echo ""


echo ""
echo "=============== BUILD (make) BEGIN"
make -C cmake_build_dir
echo "=============== BUILD (make) END"
echo ""

echo ""
echo "=============== RUN (bin) BEGIN"
cmake_build_dir/bin
echo "=============== RUN (bin) END"
echo ""

echo ""
echo "=============== contenu de compile_commands.json"
cat cmake_build_dir/compile_commands.json
echo "=============== RUN (bin) END"
echo ""
