#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

rm -rf _build_dir
rm -rf subprojects  # le nom est imposé par le package-manager de meson (=wrap)

echo ""
echo "=============== SETUP GOOGLETEST DEPENDENCY BEGIN"
mkdir subprojects
meson wrap install gtest
echo "=============== SETUP GOOGLETEST DEPENDENCY END"
echo ""

# à ce stade, subprojects/gtest.wrap contient les infos permettant de télécharger googletest
# (mais googletest n'est PAS encore téléchargé)

# de façon intéressante, googletest ne fournit pas de fichier de configuratio meson, du coup
# le package est constitué de deux archives :
#    la librairie googletest
#    un patch pour lui ajouter un fichier de configuration meson

echo ""
echo "=============== CONFIGURE BEGIN"
meson setup _build_dir/ .
echo "=============== CONFIGURE END"
echo ""

# à ce stade, meson a téléchargé googletests :
#    les archives compressées sont dans subprojects/packagecache
#    l'archive décompressée est dans subprojects/googletest-1.14.0

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

Cette POC montre l'utilisation pas-à-pas de googletest avec meson.

Piqué de cette vidéo = https://www.youtube.com/watch?v=Tudwcpxpr6c

En gros :
    - création du répertoire subprojects obligatoire
    - meson wrap install gtest
    - utilisation des objets fournis par gtest.wrap dans le meson.build
    - définition d'un exécutable de test dans meson.build
    - meson configure télécharge la lib
    - meson compile ou test l'utilise

NOTE : côté gitignore, tout le contenu de subprojects doit être ignoré SAUF les fichiers .wrap :
    /subprojects/*
    !/subprojects/*.wrap

En effet, les fichiers .wrap sont la spec des dépendances du projet, et le reste du répertoire subprojects est du
contenu automatiquement géré par meson.
EOF
