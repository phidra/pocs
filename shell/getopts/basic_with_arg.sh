#!/bin/bash
########################################################################################################################
# POC sur l'utilisation de getopts pour parser des arguments.
#
# Cas presque le plus simple : une seule option possible (-a), qui requiert un argument.
#
# Les appels VALIDES déclenchant le flag a :
#    ./basic_with_arg.sh -a ARG
#    ./basic_with_arg.sh -a=ARG
#    ./basic_with_arg.sh -aARG
#    ./basic_with_arg.sh -a ARG POUET1 POUET2
#    ./basic_with_arg.sh -a -b  (il considère que -b n'est pas une OPTION, mais l'ARGUMENT de l'option -a)
# 
# Les appels VALIDES, mais NE DÉCLENCHANT PAS le flag a :
#    ./basic_with_arg.sh POUET1 -a ARG POUET2
#    ./basic_with_arg.sh POUET1 POUET2 -a ARG
# 
# Les appels INVALIDES :
#    ./basic_with_arg.sh -a
#    ./basic_with_arg.sh -b
# 
########################################################################################################################

while getopts "a:" opt
do
    case $opt in
    a)
        echo "Le flag  '-a'  a été passé en option, avec le paramètre : \"${OPTARG}\""
    ;;
    \?) # Si getopt rencontre une erreur, il settera $opt à "?"
        echo "Option invalide : -$OPTARG"
    ;;
    esac
done

########################################################################################################################
# Fin du fichier.
########################################################################################################################

