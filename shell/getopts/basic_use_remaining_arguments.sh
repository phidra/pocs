#!/bin/bash
########################################################################################################################
# POC sur l'utilisation de getopts pour parser des arguments.
#
# Démonstration de comment utiliser le reste des arguments (ceux qui ne correspondaient pas à une option).
#
# Les appels à essayer, SANS argument supplémentaires :
#    ./basic_use_remaining_arguments.sh 
#    ./basic_use_remaining_arguments.sh -b POUET1
#    ./basic_use_remaining_arguments.sh -b POUET1 -c POUET2
#    ./basic_use_remaining_arguments.sh -b POUET1 -c POUET2 --
#
# Les appels à essayer, AVEC argument supplémentaires :
#    ./basic_use_remaining_arguments.sh ARG1 ARG2
#    ./basic_use_remaining_arguments.sh -b POUET1 ARG1 ARG2
#    ./basic_use_remaining_arguments.sh -b POUET1 -c POUET2 ARG1 ARG2
#    ./basic_use_remaining_arguments.sh -b POUET1 -c POUET2 -- ARG1 ARG2
#
# Issu de http://stackoverflow.com/a/14203146
# 
########################################################################################################################

OPTIND=1

while getopts "hHb:c:" opt
do
    case $opt in
    h|H)
        echo "Je suis l'aide du script."
        exit 0
        ;;
    b)  echo "Le flag b est activé, avec l'argument : ${OPTARG}"
        ;;
    c)  echo "Le flag c est activé, avec l'argument : ${OPTARG}"
        ;;
    \?) exit 42
        ;;
    esac
done

# C'est ici que c'est intéressant, puisque c'est ce code qu'il faut utiliser pour traiter la suite des arguments :
shift $((OPTIND-1))
[ "$1" = "--" ] && shift

if [ "$1" == "" ]
then
    echo "Une fois les options traitées, il n'y a pas d'arguments !"
else
    echo "La suite des arguments (qui commence avec \"$1\") est :"
    for i in "$@"
    do
        echo "    - $i"
    done
fi

########################################################################################################################
# Fin du fichier.
########################################################################################################################
