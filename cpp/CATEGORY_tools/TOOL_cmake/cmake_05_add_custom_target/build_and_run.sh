#!/bin/bash

set -o errexit
set -o nounset

echo """
This POC show the use of add_custom_command / add_custom_target / add_dependencies

In this POC, our custom command builds a header file BEFORE building binary.
NOTE : it seems that add_custom_target is preferable.
NOTE : the custom_command is executed at build-time.

cmake -Bcmake_build_dir  -H.

make -C cmake_build_dir

"""

rm -rf cmake_build_dir


echo ""
echo "===== CONFIGURE (cmake) BEGIN"
cmake -Bcmake_build_dir -H.
rm -f src/value.h
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
