#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

rm -rf _build_dir
rm -rf subprojects  # le nom est imposé par le package-manager de meson (=wrap)

echo ""
echo "=============== SETUP CATCH2 DEPENDENCY BEGIN"
mkdir subprojects
meson wrap install catch2
echo "=============== SETUP CATCH2 DEPENDENCY END"
echo ""

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
echo "=============== TEST EXECUTION THROUGH MESON BEGIN"
meson test -C _build_dir/
echo "=============== TEST EXECUTION THROUGH MESON END"
echo ""

echo ""
echo "=============== MANUAL TEST EXECUTION BEGIN"
_build_dir/my_super_test
echo "=============== MANUAL TEST EXECUTION END"
echo ""




cat << EOF

Cette POC montre l'utilisation pas-à-pas de catch2 avec meson.

C'est un clone de la POC équivalente avec googletest.

NOTE : il se peut que je m'y prenne mal, car on dirait que les tests du projet catch2 sont lancés avec les miens...

EOF
