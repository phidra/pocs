#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

cat << EOF

POC utilisant snowpack pour builder une app frontend avec une dépendance (moment.js) et du code typescript

Lancement du serveur de dev :
    npm install
    npm run dev

Builder le site, et le servir avec un serveur python classique :
    npm install
    npm run build
    python3 -m http.server --directory='${this_script_parent}/dist/' 8787

(dans les deux cas, le serveur indique l'URL à requêter pour utiliser le site)

Lancer la compilation typescript manuellement :
    npm run compile

Note : le code de la POC comporte volontairement une erreur typescript pour tester sa bonne détection.

---

L'intérêt de cette POC par rapport aux deux précédentes est de compiler le typescript, via @snowpack/plugin-typescript.
En revanche, j'ai rencontré un bug de compilation au build :
    quand je laisse une erreur typescript dans le code :
        OK = npm run dev détecte bien l'erreur et l'affiche, mais n'empêche pas le serveur de dev de se lancer
        OK = npm run build détecte bien les erreurs et fait échouer le build...
    quand je fixe l'erreur :
        OK = npm run dev indique correctement qu'il n'y a pas d'erreur
        OK = npm run build builde correctement sans erreur
    le truc pas cool : quand je réintroduis l'erreur :
        OK = npm run dev détecte bien les erreurs, mais n'empêche pas le serveur de dev de se lancer
        KO !!! npm run build ne détecte plus l'erreur ! (et considère que le build est en succès)

Du coup, pour contourner le problème, je ne build qu'après compilation dans le package.json :
    "prebuild": "tsc --noEmit",
    "build": "snowpack build",
De plus, j'ajoute une cible explicite pour compiler manuellement dans le package.json :
        "compile": "tsc --noEmit"


Plus généralement, la POC aboutit à un setup minimal utilisable :
    utilisation de '@snowpack/plugin-typescript' pour que le live-server compile typescript
    quelques options de config snowpack utiles (e.g. devOptions.open: "none")
    utilisation d'un sous-module typescript (et comment l'importer malgré les extensions ts/js)
    les scripts utiles dans package.json pour contourner le problème de compilation

EOF
