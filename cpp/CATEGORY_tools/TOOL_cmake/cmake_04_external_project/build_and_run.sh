#!/bin/bash

set -o errexit
set -o nounset

echo """
This POC show the use of external_project to download an archive in a lib directory.

The content of the archive is used as a header-only library.

NOTE : the archive is NOT downloaded by cmake at configure-time.
it is downloaded by make at build-time.

cmake -Bcmake_build_dir  -H.

make -C cmake_build_dir

"""

rm -rf cmake_build_dir
rm -rf lib_GITIGNORE

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
