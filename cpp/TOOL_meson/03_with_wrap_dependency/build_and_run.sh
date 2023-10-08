#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

echo ""
echo "POC issue de :"
echo "    https://mesonbuild.com/GuiTutorial.html"
echo ""
echo "Fait manuellement :"
echo "    mkdir subprojects"
echo "    meson wrap install sdl2"
echo ""
echo "ATTENTION : l'exécution nécessite un environnement graphique."
echo ""
echo ""




set -o xtrace
meson setup build
meson compile -C build
build/pouet
