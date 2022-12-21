#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

echo ""
echo ""
echo "POC utilisant parcel pour builder une app frontend avec une dépendance (moment.js) et du code typescript"
echo ""
echo "NOTE : dans le HTML, on peut inclure directement le typescript, parcel se débrouille pour inclure plutôt du js au final."
echo ""
echo "Lancement du serveur de dev :"
echo "    npm install"
echo "    npm run dev"
echo ""
echo "Builder le site, et le servir avec un serveur python classique :"
echo "    npm install"
echo "    npm run build"
echo "    python3 -m http.server --directory='${this_script_parent}/dist/' 8787"
echo ""
echo "(dans les deux cas, le serveur indique l'URL à requêter pour utiliser le site)"
echo ""
