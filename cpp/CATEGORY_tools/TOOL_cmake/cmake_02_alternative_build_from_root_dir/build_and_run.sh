#!/bin/bash

set -o errexit
set -o nounset

cat << EOF
This POC show how to launch cmake without having to go into a _build dir first.

cmake -Bdir1  -Hdir2  # build "dir2" into "dir1"

make -C dir1  # launch make from "dir1" directory

EOF

rm -rf _build


echo ""
echo "===== CONFIGURE (cmake) BEGIN"
cmake -B_build -H.
echo "===== CONFIGURE (cmake) END"
echo ""


echo ""
echo "===== BUILD (make) BEGIN"
make -C _build
echo "===== BUILD (make) END"
echo ""

echo ""
echo "===== RUN (bin) BEGIN"
chmod +x _build/bin
_build/bin
echo "===== RUN (bin) END"
echo ""
