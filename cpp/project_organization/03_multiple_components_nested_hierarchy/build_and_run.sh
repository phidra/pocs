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
echo "=============== TESTS BEGIN"
meson test -C _build_dir/
echo "=============== TESTS END"
echo ""

echo ""
echo "=============== RUN BEGIN"
# submain of person lib :
_build_dir/person/get-person-status luke 42
_build_dir/person/get-person-status young-anakin 8

# project's main :
set +o errexit
_build_dir/phonebook/cli-phonebook
set -o errexit
echo "=============== RUN END"
echo ""

cat << EOF

Ce que montre cette POC = organisation d'un projet multi-composants, à hiérarchie non-flat.


Par rapport à la POC précédente :

- on n'a plus un main global, mais plutôt un composant PRINCIPAL (phonebook)
    (du coup, c'est le main de ce composant qui joue le rôle de main global)
- le composant phonebook dépend de :
    - persons (au même niveau que lui)
    - personal_infos/address (nested sous personal_infos)
    - personal_infos/phone (nested sous personal_infos)

Quelques notes vrac :

- pour simplifier le propos, j'ai mis moins de tests d'implémentations privées
- pour simplifier le propos, j'ai mis moins de sous-mains
- pour que la hiérarchie nested apparaisse dans les includes, il faut complexifier encore un coup le répertoire "public"e
- j'ai rassemblé les tests des API publiques de tous les composants dans un mêm répertoire à part
- j'ai simplifié la CLI du phonebook (pour la rendre interactive pour la POC) mais en vrai, il y aurait de l'interaction

À noter que personal_infos pourrait être un peu plus qu'un container vide, il pourrait être un composant parent, avec du code.
Ça revient à ajouter le quatuor "habituel" à personal_nifos (en plus de personal_infos/address et personal_infos/phone) :
    - personal_infos/public
    - personal_infos/impl
    - personal_infos/tests
    - personal_infos/mains dans l

EOF
