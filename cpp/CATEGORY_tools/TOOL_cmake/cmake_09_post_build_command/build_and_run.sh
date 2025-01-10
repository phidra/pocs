#!/bin/bash

set -o errexit
set -o nounset

echo """
This POC show the usage of a POST_BUILD command to copy the built binary in the source directory.

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
# le binaire a été déplacé par cmake au même niveau que les sources :
chmod +x mybinary
./mybinary
echo "===== RUN (bin) END"
echo ""
