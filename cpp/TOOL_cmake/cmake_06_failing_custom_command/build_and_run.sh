#!/bin/bash

set -o errexit
set -o nounset

echo """
This POC shows that when a custom command fails, all the build is failed.

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
chmod +x cmake_build_dir/bin
cmake_build_dir/bin
echo "===== RUN (bin) END"
echo ""
