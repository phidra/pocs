#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

echo ""
echo "POC issue de :"
echo "    https://mesonbuild.com/Tutorial.html"
echo "Si besoin :"
echo "    sudo apt install libgtk-3-dev"
echo "ATTENTION : l'exécution nécessite un environnement graphique."
echo ""
echo ""


set -o xtrace
meson setup build
meson compile -C build
build/pouet
