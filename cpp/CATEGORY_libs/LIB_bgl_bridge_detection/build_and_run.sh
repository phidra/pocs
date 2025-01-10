#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
# set -o xtrace

this_script_parent="$(realpath "$(dirname "$0")" )"


# les explications de la POC sont en header du script "run.sh"
cat << EOF
Dépendances au runtime :

Disposer d'un extract osm :
    wget -O NOGIT_extract.osm.pbf "https://download.bbbike.org/osm/bbbike/Bordeaux/Bordeaux.osm.pbf"
    wget -O NOGIT_extract.osm.pbf "https://download.geofabrik.de/europe/monaco-latest.osm.pbf"

EOF

"${this_script_parent}/clean.sh"
"${this_script_parent}/build.sh"
"${this_script_parent}/download.sh"
"${this_script_parent}/run.sh"
