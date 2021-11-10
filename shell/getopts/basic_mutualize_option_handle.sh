#!/bin/bash
########################################################################################################################
# POC sur l'utilisation de getopts pour parser des arguments.
#
# Cas où on souhaite mutualiser le traitement d'options (ici, on traite de la même façon les options "h" et "H").
#
# Les appels VALIDES :
#    ./basic_mutualize_option_handle.sh -h
#    ./basic_mutualize_option_handle.sh -h POUET1 POUET2
#    ./basic_mutualize_option_handle.sh -H
#    ./basic_mutualize_option_handle.sh -H POUET1 POUET2
# 
# Les autres appels sont soit invalides, soit ne déclenchent pas le traitement mutualisé.
# 
########################################################################################################################

function show_help() { echo "Je suis l'aide du script, affichée si l'utilisateur le demande ou en cas d'erreur."; }

while getopts "hH" opt
do
    case $opt in
    h|H)
        show_help
        exit 0
    ;;
    \?)
        # Si l'option n'est pas reconnue, on montre tout de même l'aide, mais on sort en erreur.
        show_help
        exit 1
    ;;
    esac
done

########################################################################################################################
# Fin du fichier.
########################################################################################################################
