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
_build_dir/person/get-person-status luke 42
_build_dir/person/get-person-status young-anakin 8
echo "=============== RUN END"
echo ""

cat << EOF

Ce que montre cette POC = organisation simple d'un projet.

Notes vrac :

STRUCTURE :
- le projet se présente sous la forme d'une lib (et d'un main qui est utilisateur de la lib)
- le projet est mono-compoant : une seule lib
- il présente la structure de base d'un composant, en 4 répertoires :
    - public  = l'API publique
    - impl    = l'implémentation privée
    - tests   = les tests
    - mains   = les mains du composant
- cette structure sera la même pour chaque composant d'un projet plus complexes multi-composants

TESTS :
- on distingue deux types de tests :
    - tests de l'API publique = des clients comme les autres de la lib
    - tests de trucs privés   = teste des trucs internes à la lib (connaît de trucs inaccessibles aux clients externes)
- ici, les deux types de tests sont regroupés ensemble dans le répertoire tests
- mais les tests de l'API publique pourraient ne même pas être dans la structure de répertoires du composant
- (c'est ce que je fais dans la POC suivante)
- détail important : les tests de l'implémentation privée ont des super-pouvoirs = accéder aux trucs privés

MAINS :
- ici, le main utilise simplement la librairie (donne la boisson qu'une personne a le droit de boire)
- dans le POC suivante, person est un composant parmi d'autres -> le projet a ses mains, et la lib son propre sous-main

EOF
