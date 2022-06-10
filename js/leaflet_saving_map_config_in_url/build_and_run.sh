#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

echo ""
echo "Ce script lance un serveur pour servir la POC leaflet (nécessaire, car j'utilise des modules js)"
echo "Derrière, il faut charger l'URL indiquée par le serveur dans un browser."
echo "    python3 -m http.server --directory='${this_script_parent}/dist/' 8787"
echo ""
echo "(derrière, le serveur indique l'URL à requêter pour utiliser le site)"
python3 -m http.server --directory="${this_script_parent}" 8787
