#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

rm -rf _build_dir


echo ""
echo "=============== CONFIGURE BEGIN"
meson setup _build_dir/ .
echo "=============== CONFIGURE END"
echo ""

echo ""
echo "=============== BUILD BEGIN"
meson compile -C _build_dir/
echo "=============== BUILD END"
echo ""

echo ""
echo "=============== MANUAL TEST EXECUTION BEGIN"
_build_dir/my_super_bin
echo "=============== MANUAL TEST EXECUTION END"
echo ""




cat << EOF

POC d'utilisation basique de nanobench = https://nanobench.ankerl.com/

Cf. mes notes markdown.

Plusieurs outputs sont générés :
    console
    html
    csv

La fonction que je perfteste se contente de sleep 100ms.

Le champ ns/op indique le temps moyen nécessaire à l'exécution (on y retrouve donc logiquement une valeur d'environ 100ms)
EOF

