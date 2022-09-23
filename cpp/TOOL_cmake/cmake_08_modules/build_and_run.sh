#!/bin/bash

set -o errexit
set -o nounset

echo """
This POC shows how to include and use a cmake module.
Here, we use CMakePrintHelpers module to display two variables at conf-time : DIRECTION and ANGLE.


"""

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
