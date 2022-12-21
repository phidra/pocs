#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

echo ""
echo "Pour cette POC, il est indispensable de passer par un serveur : on ne peut pas simplement charger le fichier HTML"
echo "dans un browser, sous peine d'avoir une erreur CORS car on utilise des modules js."
echo ""
echo "Le moyen le plus simple est de passer par le runner vue.js qui permet de lancer un serveur de dev :"
echo "    npm run dev"
echo ""
echo "En alternative, on peut builder le site, et le servir avec un serveur python classique :"
echo "    npm install"
echo "    npm run build"
echo "    python3 -m http.server --directory='${this_script_parent}/dist/' 8787"
echo ""
echo "(dans les deux cas, le serveur indique l'URL à requêter pour utiliser le site)"
