#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

echo ""
echo ""
echo "POC utilisant snowpack pour builder une app frontend avec une dépendance (moment.js) et du code typescript"
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
echo "Par rapport à parcel :"
echo "    - nécessite une config (par rapport à parcel qui fonctionne out of the box), mais rien de très compliqué"
echo "    - le live-server semble fonctionner correctement (pas comme parcel), et c'est assez rapide, même sans cache"
echo "    - sans plugin, il n'y a PAS de typecheking typescript, uniquement la transpilation !"
echo "    - on peut importer des modules relatifs typescript, en n'utilisant pas leur extension :"
echo "        import {log_hoho} from './youpi/hoho';"
echo "    - on dirait que pour que le HTML utilise le TS, je peux me contenter d'utiliser un équivalent js direct :"
echo '        <script type="module" src="app.js"></script>'
echo "        (tout se passe comme si mon TS est en fait du JS, on dirait)"
echo ""
