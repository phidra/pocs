#!/bin/bash
########################################################################################################################
# POC sur l'utilisation de getopts pour parser des arguments.
#
# Cas le plus simple : une seule option possible (-a), qui ne requiert pas d'argument.
#
# Les appels VALIDES déclenchant le flag a :
#    ./basic.sh
#    ./basic.sh POUET1 POUET2
#    ./basic.sh -a
#    ./basic.sh -a POUET1 POUET2
# 
# Les appels VALIDES, mais NE DÉCLENCHANT PAS le flag a :
#    ./basic.sh POUET1 -a POUET2
#    ./basic.sh POUET1 POUET2 -a
# 
# Les appels INVALIDES :
#    ./basic.sh -b
# 
########################################################################################################################

while getopts "a" opt # si on ne précise pas de troisième argument à getopt, il utilise par défaut "$@"
do
    case $opt in
    a)
        echo "Le flag  '-a'  a été passé en option !"
    ;;
    \?) # Si getopt rencontre une erreur, il settera $opt à "?"
        echo "Option invalide : -$OPTARG"
    ;;
    esac
done

########################################################################################################################
# Fin du fichier.
########################################################################################################################
