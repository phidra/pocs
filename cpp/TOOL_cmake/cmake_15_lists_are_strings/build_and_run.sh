#!/bin/bash

set -o errexit
set -o nounset

echo """
This POC shows that the generation by cmake of compile_commands.json, which helps ALE to work.

The focus of this POC is configure-time.

"""

rm -rf cmake_build_dir

echo ""
echo "===== CONFIGURE (cmake) BEGIN"
cmake -Bcmake_build_dir -H.
echo "===== CONFIGURE (cmake) END"
echo ""


echo ""
echo "===== BUILD (make) BEGIN"
make -C cmake_build_dir
echo "===== BUILD (make) END"
echo ""

echo ""
echo "===== RUN (bin) BEGIN"
cmake_build_dir/bin
echo "===== RUN (bin) END"
echo ""
