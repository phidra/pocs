#!/bin/bash
########################################################################################################################
# POC sur l'utilisation de getopts pour parser des arguments.
#
# Cas où on souhaite rendre une option obligatoire (il faut le gérer à la main).
#
# Les appels VALIDES :
#    ./basic_with_required_option.sh -a
#    ./basic_with_required_option.sh -a POUET1 POUET2
# 
# Les appels INVALIDES :
#    ./basic_with_required_option.sh
#    ./basic_with_required_option.sh POUET1 POUET2
#    ./basic_with_required_option.sh POUET1 -a POUET2
#    ./basic_with_required_option.sh POUET1 POUET2 -a
#    ./basic_with_required_option.sh -b
# 
########################################################################################################################

is_a_set=0
while getopts "a" opt
do
    case $opt in
    a)
        echo "Le flag  '-a'  a été passé en option !"
        is_a_set=1
    ;;
    \?)
        exit 31 # On sort avec un code différent du code indiquant que l'option a manque.
    ;;
    esac
done

[ "${is_a_set}" == "0" ] && echo "ERROR - option a is required !" && exit 42

########################################################################################################################
# Fin du fichier.
########################################################################################################################
