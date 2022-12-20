#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

Cette POC est simplement le support du suivi de ce tutorial sur typescript :
    https://www.typescripttutorial.net/typescript-tutorial/typescript-hello-world/

Ici, l'app typescript est intégrée à une page web.

EOF

this_script_parent="$(realpath "$(dirname "$0")" )"
rm -f NOGIT_app.js

tsc app.ts --outFile NOGIT_app.js

echo ""
echo "VIEW POC WITH : firefox file://${this_script_parent}/index.html"
