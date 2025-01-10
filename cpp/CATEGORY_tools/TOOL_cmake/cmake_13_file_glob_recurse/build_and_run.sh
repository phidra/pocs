#!/bin/bash

set -o errexit
set -o nounset

echo """
This POC mainly shows the use of file + GLOB_RECURSE to find sources to build.

cpp files are scattered in some subfolders, file + GLOB_RECURSE makes it easy to find them.

This POC also show a function definition + list manipulation.

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
echo "===== RUN (bin1 + bin2 + bin3) BEGIN"
cmake_build_dir/bin1
echo ""
cmake_build_dir/bin2
echo ""
cmake_build_dir/bin3
echo "===== RUN (bin) END"
echo ""
