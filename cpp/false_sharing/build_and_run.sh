#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
# set -o xtrace


cat << EOF
Cette POC montre le false_sharing en action.

CE QUE FAIT LA POC :
    - elle lance deux calculs identiques et assez intensifs dans deux threads
    - elle mesure le temps nécessaire pour que les deux threads terminent
    - 3 version de la POC existent :
        - slow = chaque thread travaile directement sur la variable résultat, et les deux variables sont contigües en mémoire
        - fast = chaque thread travaile directement sur la variable résultat, mais les deux variables sont séparées en mémoire
        - reference = chaque thread travaille sur une variable locale, et ne mets à jour la variable résultat qu'une fois, en fin de calcul

SORTIE TYPIQUE :
    +++ running SLOW implementation :
    real 11.69
    user 22.62
    sys 0.00

    +++ running FAST implementation :
    real 3.97
    user 7.82
    sys 0.00

    +++ running REFERENCE implementation (= the fastest) :
    real 3.11
    user 6.20
    sys 0.00

CE QUE J'EN DÉDUIS :
    - l'implémentation SLOW est lente car il y a du false sharing entre les variables, qui sont sur la même ligne de cache
    - la contrainte d'alignement de l'implémentation FAST annule ce false sharing, ce qui accélère les traitements
    - curieusement, FAST n'est pas aussi rapide que la RÉFÉRENCE (peut-être false sharing sur des cache lines de plus haut niveau ?)
--------------------------------------------------------------------------------
EOF


echo ""
echo "+++ compiling :"
g++ -std=c++14 -pthread reference.cpp -O0 -o bin_reference.GITIGNORE
g++ -std=c++14 -pthread slow.cpp -O0 -o bin_slow.GITIGNORE
g++ -std=c++14 -pthread fast.cpp -O0 -o bin_fast.GITIGNORE


ITERATION_NUMBER=1000000000
echo ""
echo "+++ ITERATION_NUMBER=${ITERATION_NUMBER}"
echo "    à adapter en fonction de la patate de la machine qui fait tourner la POC"
echo "    pour que la POC soit intéressante, l'implémentation de référence doit tourner en quelques secondes"

echo ""
echo "+++ running SLOW implementation :"
/usr/bin/time -p ./bin_slow.GITIGNORE $ITERATION_NUMBER

echo ""
echo "+++ running FAST implementation :"
/usr/bin/time -p ./bin_fast.GITIGNORE $ITERATION_NUMBER

echo ""
echo "+++ running REFERENCE implementation (= the fastest) :"
/usr/bin/time -p ./bin_reference.GITIGNORE $ITERATION_NUMBER

