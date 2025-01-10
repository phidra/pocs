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

# submain of phonebook lib :
_build_dir/phonebook/list-alphabetically luke leia anakin yoda sidious

# project's main :
set +o errexit
_build_dir/cli-phonebook
set -o errexit
echo "=============== RUN END"
echo ""

cat << EOF

Ce que montre cette POC = organisation d'un projet multi-composants.

C'est la suite de la précédente qui présentait la structure d'UN SEUL composant.
Ici, on a DEUX composants :
    - person (qui n'a pas de dépendance)
    - phonebook (qui dépend de person)

De plus, on a un main global au projet qui UTILISE les deux composants.

QUELQUES NOTES VRAC :

- chaque composant garde les propriétés de sa structure :
    - headers publics vs. implémentation privée
    - tests de l'implémentation privée vs. tests de l'API publique
    - possibilité d'avoir un sub-main propre à chaque composant
- le main du projet est "externe", dans le sens où il n'appartient à aucun composant ; il représente le projet
- (dans la vraie vie, on aura plutôt l'un des composants du projets qui sera le composant principal)
- à la différence de la POC précédente, j'ai SORTI les tests des APIs publiques des composants
- (ils sont dans un répertoire "tests" à la racine du projet, et non plus dans chaque composant)
- le fichier phonebook/meson.build PEUT utiliser des variables définies dans person/meson.build, à condition que l'ordre des fichiers soit bon


EOF
