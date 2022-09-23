#!/bin/bash

set -o errexit
set -o nounset

echo """
This POC show the most basic cmake usage.

"""

# cmake produit tout un tas de fichiers sales -> on fait le boulot depuis un répertoire trashable :
rm -rf _build
mkdir _build

pushd _build

echo ""
echo "===== CONFIGURE (cmake) BEGIN"
cmake ..
echo "===== CONFIGURE (cmake) END"
echo ""


echo ""
echo "===== BUILD (make) BEGIN"
make
echo "===== BUILD (make) END"
echo ""

popd

echo ""
echo "===== RUN (bin) BEGIN"
chmod +x _build/bin
_build/bin
echo "===== RUN (bin) END"
echo ""
