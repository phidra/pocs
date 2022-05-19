#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

echo ""
echo "Ce script lance un serveur pour servir la POC (nécessaire pour éviter une erreur CORS sur le chargement des modules js)"
echo "Derrière, il faut charger l'URL suivante dans un browser :"
echo ""
echo "     http://localhost:8787/"
echo ""
python3 -m http.server --directory="${this_script_parent}" 8787
